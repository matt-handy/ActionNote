#include <dossier.h>
#include <sstream>
#include "aes.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace handy::action_note;

uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

void dossier::set_idx(int v_idx) {
	std::stringstream filename_builder;
	idx = v_idx;
	filename_builder << setw(5) << setfill('0') << idx << ".txt";
	filename = filename_builder.str();
}

void dossier::append_information(std::string new_info) {
	information.append(new_info);
	information.append(LINE_RETURN);
}

string dossier::to_string() {
	std::stringstream myfile;
	myfile << FIRST_NAME << first_name << LINE_RETURN;
	myfile << MIDDLE_NAMES << middle_names << LINE_RETURN;
	myfile << LAST_NAME << last_name << LINE_RETURN;
	myfile << DOB << dob << LINE_RETURN;
	myfile << information;
	return myfile.str();
}

//TODO: migrate encryption code code into something common between meetings and dossiers
void dossier::to_file(string filename, uint8_t* key) {
	string myfile = to_string();

	if (key) {
		struct AES_ctx ctx;

		std::string testString = myfile;
		size_t len = testString.length() + 1; //Figure out how much data I have
		size_t delta = 16 - (len % 16); //figure out how much more data I need to get to X * 16
		len += delta; //Round up buffer to nearest 16th
		unsigned char* workingBuff = new unsigned char[len];
		memset(workingBuff, 0, len);
		memcpy(workingBuff, testString.c_str(), testString.length());

		//TODO: User a better IV
		AES_init_ctx_iv(&ctx, key, iv);
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
		diskfile << myfile;
		diskfile.close();
	}
}

void dossier::from_file(istream* input, dossier& dossier) {
	string line;
	//First name
	getline(*input, line);
	string first_name = line.substr(FIRST_NAME.length());
	dossier.update_first_name(first_name);
	//Middle names
	getline(*input, line);
	string middle_names = line.substr(MIDDLE_NAMES.length());
	dossier.update_middle_names(middle_names);
	//Last name
	getline(*input, line);
	string last_name = line.substr(LAST_NAME.length());
	dossier.update_last_name(last_name);
	//DOB
	getline(*input, line);
	string dob = line.substr(DOB.length());
	dossier.update_dob(dob);

	std::stringstream information;
	while (getline(*input, line))
	{
		information << line << LINE_RETURN;
	}
	dossier.set_information(information.str());
}

dossier dossier::from_file(string filename, uint8_t* key) {
	dossier n_dossier;
	istream* target_stream;

	if (key == NULL) {
		ifstream myfile(filename);
		if (myfile.is_open())
		{
			target_stream = &myfile;
			from_file(target_stream, n_dossier);
		}
		else {
			//TODO: throw exception
			cout << "Unable to open dossier file: " << filename;
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
			AES_CBC_decrypt_buffer(&ctx, (uint8_t*)buffer, size);
			std::stringbuf ret_buffer;
			std::ostream r_os(&ret_buffer);
			std::istream r_is(&ret_buffer);

			r_os << buffer;
			from_file(&r_is, n_dossier);
		}
		file.close();
		delete[] buffer;
	}


	string sub_filename = filename.substr(0, filename.length() - 4);
	//TODO: this is an ugly hack, we need a better detection of idx
	if (sub_filename.length() != 5) {
		sub_filename = sub_filename.substr(filename.length() - 6, 5);
	}
	int idx = std::atoi(sub_filename.c_str());
	n_dossier.set_idx(idx);

	return n_dossier;
}