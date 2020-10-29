#include "action_item.h"
#include "jstring.h"
#include "aes.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace handy::action_note;
using namespace handy::common;


bool action_item::save_as(string filename, uint8_t* key) {
	string action_content = render_text();
	if (key) {
		struct AES_ctx ctx;

		size_t len = action_content.length() + 1; //Figure out how much data I have
		size_t delta = 16 - (len % 16); //figure out how much more data I need to get to X * 16
		len += delta; //Round up buffer to nearest 16th
		unsigned char* workingBuff = new unsigned char[len];
		memset(workingBuff, 0, len);
		memcpy(workingBuff, action_content.c_str(), action_content.length());

		AES_init_ctx_iv(&ctx, key, handy::action_note::iv);
#ifdef DEBUG
		cout << "Encrypting: " << len << endl;
#endif
		AES_CBC_encrypt_buffer(&ctx, (uint8_t*)workingBuff, len);
#ifdef DEBUG
		cout << "Writing to file: " << filename << endl;
#endif

		std::ofstream file(filename, std::ios::binary);
		file.write((const char*)workingBuff, len);
		file.close();
		delete[] workingBuff;
	}
	else {
		ofstream diskfile;
		diskfile.open(filename);
		diskfile << action_content;
		diskfile.close();
	}

	file = filename;
	return true;
}

bool action_item::save(uint8_t* key) {
	if (file.size() != 0) {
		save_as(file, key);
		return true;
	}
	else {
		return false;
	}
}

action_item* action_item::get_action_item(string filename, uint8_t* key) {
	string line;
	action_item* ai = NULL;
	istream* target_stream;

	if (key == NULL) {
		ifstream myfile(filename);
		if (myfile.is_open())
		{
			target_stream = &myfile;
			if (getline(*target_stream, line)) {
				ai = get_action_item(line, target_stream, NULL);
			}
		}
		else {
			cout << "Unable to open action item file";
			return NULL;
		}

		myfile.close();
	}
	else {
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		char* buffer = new char[size];
		//cout << "reading into buffer: " << size << endl;
		if (file.read(buffer, size))
		{
			struct AES_ctx ctx;
			AES_init_ctx_iv(&ctx, key, iv);
			AES_CBC_decrypt_buffer(&ctx, (uint8_t*)buffer, size);
			//cout << "Decrypted:" << size << endl;
			std::stringbuf ret_buffer;
			std::ostream r_os(&ret_buffer);
			std::istream r_is(&ret_buffer);

			r_os << buffer;
			target_stream = &r_is;
			if (getline(*target_stream, line)) {
				ai = get_action_item(line, target_stream, NULL);
			}
		}
		file.close();
		delete[] buffer;
	}

	if (ai != NULL) {
		ai->set_filename(filename);
	}
	return ai;
}

action_item* action_item::get_action_item(string line, istream* stream, meeting* parent) {
	string curr_ai_assignee;
	string curr_ai_start;
	string curr_ai_due;
	string curr_ai_text;
	string curr_ai_enddate;
	vector<string> ai_notes;

	vector<jstring> action_data = jstring(line.c_str()).split(jstring(" "));
	if (action_data.size() != 6 && action_data.size() != 4 && action_data.size() != 2) {
		//return bad status showing inability to parse
	}
	if (!action_data.at(0).equals("@@AI")) {//TODO: cleanup and reconcile with previous constant
		//return bad status showing invalid action
	}
	curr_ai_assignee = action_data.at(1).c_str();
	if (action_data.size() >= 4) {
		if (action_data.at(2).equals("@@start")) {
			curr_ai_start = action_data.at(3).c_str();
		}
		else if (action_data.at(2).equals("@@due")) {
			curr_ai_due = action_data.at(3).c_str();
		}
	}
	if (action_data.size() == 6) { //TODO: account for double stating start or due dates
		if (action_data.at(4).equals("@@start")) {
			curr_ai_start = action_data.at(5).c_str();
		}
		else if (action_data.at(4).equals("@@due")) {
			curr_ai_due = action_data.at(5).c_str();
		}
	}

	bool in_action_text = true;
	while (getline(*stream, line))
	{
		if (line.find(ACTION_CLOSED_TAG) != std::string::npos) {
#ifdef DEBUG
			cout << "Pushing closed tag!" << endl;
#endif
			curr_ai_enddate = line.substr(ACTION_CLOSED_TAG.size());
		}
		else if (line.find(ACTION_NOTE_TAG) != std::string::npos) {
#ifdef DEBUG
			cout << "Pushing note!" << endl;
#endif
			in_action_text = false;
			ai_notes.push_back(line.substr(ACTION_NOTE_TAG.size()));
		}
		else if (line.find(ACTION_END_TAG) != std::string::npos) {
#ifdef DEBUG
			cout << "End action tag" << endl;
#endif
			in_action_text = false;
			action_item* ai = new action_item(curr_ai_assignee, curr_ai_start, curr_ai_due, curr_ai_text, curr_ai_enddate, parent);
#ifdef DEBUG
			cout << "AI: " << ai << endl;
#endif
			for (int idx = 0; idx < ai_notes.size(); idx++) {
				ai->add_note(ai_notes.at(idx));
			}
			
			return ai;
		}
		else if (in_action_text) {
#ifdef DEBUG
			cout << "Line of action: " << line << endl;
#endif
			if (curr_ai_text.size() != 0) {
				curr_ai_text.append(LINE_RETURN);
			}

			curr_ai_text.append(line);
			
		}
		else {
			//I need to have this for making notes multi-line. TODO, add support for multilines
#ifdef DEBUG
			cout << "Adding note: " << line << endl;
#endif
			ai_notes.push_back(line.substr(ACTION_NOTE_TAG.size()));
		}
	}
	return NULL;
}

string action_item::render_text() {
	stringstream output;

	output << ACTION_TAG << assignee;
	if (start_date.size() != 0) {
		output << " " << ACTION_START_TAG << start_date;
	}
	if (due_date.size() != 0) {
		output << " " << ACTION_DUE_TAG << due_date;
	}
	output << LINE_RETURN;

	output << action_text << LINE_RETURN;

	if (notes.size() != 0) {
		for (list<string>::const_iterator i = notes.begin(); i != notes.end(); ++i)
		{
			output <<ACTION_NOTE_TAG << i->c_str() << LINE_RETURN;
		}
	}

	if (closed_date.size() != 0) {
		output << ACTION_CLOSED_TAG << closed_date << LINE_RETURN;
	}

	output << ACTION_END_TAG << LINE_RETURN;

	return output.str();
}