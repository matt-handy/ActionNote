#include "meeting.h"
#include "action_item.h"
#include "aes.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <jstring.h>

using namespace handy;
using namespace handy::common;
using namespace handy::action_note;

void meeting::to_file(string filename, uint8_t* key) {
	original_file = filename;
	std::stringstream myfile;

	myfile << ATTENDEE;
	list<string>::iterator a_iter = attendees.begin();
	bool first = true;
	while (a_iter != attendees.end()) {
		if (first) {
			first = false;
		}
		else {
			myfile << ", ";
		}
		myfile << *a_iter;
		advance(a_iter, 1);
	}

	myfile << LINE_RETURN;

	list<meeting_element*>::iterator e_iter = elements.begin();
	while (e_iter != elements.end()) {
		string element = (*e_iter)->render_text();
		myfile << element;
		
		advance(e_iter, 1);
	}

	if (key) {
		struct AES_ctx ctx;
		
		std::string testString = myfile.str();
		size_t len = testString.length() + 1; //Figure out how much data I have
		size_t delta = 16 - (len % 16); //figure out how much more data I need to get to X * 16
		len += delta; //Round up buffer to nearest 16th
		unsigned char* workingBuff = new unsigned char[len];
		memset(workingBuff, 0, len);
		memcpy(workingBuff, testString.c_str(), testString.length());

		//TODO: Create a more secure IV
		AES_init_ctx_iv(&ctx, key, handy::action_note::iv);
#ifdef DEBUG
		cout << "Encrypting: " << len << endl;
#endif
		AES_CBC_encrypt_buffer(&ctx, (uint8_t*)workingBuff, len);
#ifdef DEBUG
		cout << "Writing to file: " << filename << endl;
#endif

		std::ofstream file(filename, std::ios::binary);
		file.write((const char *)workingBuff, len);
		file.close();
		delete[] workingBuff;
	}
	else {
		ofstream diskfile;
		diskfile.open(filename);
		diskfile << myfile.str();
		diskfile.close();
	}
	
}

meeting* meeting::from_file(istream* input, meeting * meeting) {
	string line;
	string current_segment;
	bool in_action = false;
	bool in_action_text = false;
	while (getline(*input, line))
	{
		if (line.find(ATTENDEE) != std::string::npos) {
#ifdef DEBUG
			cout << "Found attendees: " << line << endl;
#endif
			string attendee_list = line.substr(ATTENDEE.size());
			jstring str = jstring((char*)attendee_list.c_str());
			jstring splitter = jstring((char*)", ");
			std::vector<jstring> attendee_v = str.split(splitter);
			for (vector <jstring> ::iterator it = attendee_v.begin(); it != attendee_v.end(); ++it) {
				meeting->attendees.push_back(it->c_str());
			}
		}
		else if (line.find(ACTION_TAG) != std::string::npos) {
#ifdef DEBUG
			cout << "Found action tag: " << line << endl;
#endif
			if (current_segment.size() != 0) {
				meeting->elements.push_back(new meeting_text(current_segment));
				current_segment = "";
			}

			action_item* ai = action_item::get_action_item(line, input, meeting);
			meeting->elements.push_back(ai);
			meeting->actions.push_back(ai);

		}

		else {

#ifdef DEBUG
			cout << "Appending to current segment: " << line << endl;
#endif
			current_segment.append(line);
			current_segment.append(LINE_RETURN);
		}
	}
	if (current_segment.size() != 0) {
		meeting->elements.push_back(new meeting_text(current_segment));
	}
}

meeting* meeting::from_file(string filename, uint8_t* key) {
	meeting* meeting = new handy::action_note::meeting();
	meeting->original_file = filename;
	istream* target_stream;

	if (key == NULL) {
		ifstream myfile(filename);
		if (myfile.is_open())
		{
			target_stream = &myfile;
			from_file(target_stream, meeting);
		}
		else {
			//TODO: throw exception
			cout << "Unable to open file";
			return NULL;
		}

		myfile.close();
	}
	else {
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		char* buffer = new char[size];
		if (file.read(buffer, size))
		{
			struct AES_ctx ctx;
			AES_init_ctx_iv(&ctx, key, iv);
			AES_CBC_decrypt_buffer(&ctx, (uint8_t *)buffer, size);
			std::stringbuf ret_buffer;
			std::ostream r_os(&ret_buffer);
			std::istream r_is(&ret_buffer);

			r_os << buffer;
			from_file(&r_is, meeting);
		}
		file.close();
		delete[] buffer;
	}
	

	

	return meeting;
}