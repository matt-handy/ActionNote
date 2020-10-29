#pragma once

#include <string>;

#include "meeting_element.h"

namespace handy {
	namespace action_note {

		const std::string FIRST_NAME = "First Name: ";
		const std::string LAST_NAME = "Last Name: ";
		const std::string MIDDLE_NAMES = "Middle Name(s): ";
		const std::string DOB = "DOB: ";

		class dossier {
		private:
			std::string first_name;
			std::string last_name;
			std::string middle_names;

			std::string dob;

			std::string information;

			int idx;
			std::string filename;

			static void from_file(istream* input, dossier& dossier);
		public:
			std::string to_string();

			dossier& operator=(dossier other)
			{
				std::swap(first_name, other.first_name);
				std::swap(middle_names, other.middle_names);
				std::swap(last_name, other.last_name);
				std::swap(dob, other.dob);
				std::swap(information, other.information);
				std::swap(filename, other.filename);
				std::swap(idx, other.idx);
				return *this;
			}

			void set_idx(int v_idx);

			int get_idx() {
				return idx;
			}

			std::string get_filename() {
				return filename;
			}

			std::string get_middle_names() {
				return middle_names;
			}

			void update_middle_names(std::string v_middleNames) {
				middle_names = v_middleNames;
			}

			std::string get_first_name() {
				return first_name;
			}

			void update_first_name(std::string v_firstName) {
				first_name = v_firstName;
			}

			std::string get_last_name() {
				return last_name;
			}

			void update_last_name(std::string v_lastName) {
				last_name = v_lastName;
			}

			std::string get_dob() {
				return dob;
			}

			void update_dob(std::string v_dob) {
				dob = v_dob;
			}

			std::string get_information() {
				return information;
			}

			void append_information(std::string information);

			void set_information(std::string v_information) {
				information = v_information;
			}

			void to_file(std::string filename, uint8_t* key);

			static dossier from_file(string filename, uint8_t* key = NULL);
		};
	}

}