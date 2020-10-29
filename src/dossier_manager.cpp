#include "dossier_manager.h"
#include "jstring.h"
#include <sstream>
#include <filesystem>
#include <iostream>

using namespace std;
using namespace handy::common;
using namespace handy::action_note;

const string TMP_FILENAME = "tmp_dossier.txt";

dossier_manager::dossier_manager(string m_dir, uint8_t* m_key) {
	dir = m_dir;
	key = m_key;
}

void dossier_manager::initialize() {
	for (auto& p : filesystem::directory_iterator(dir)) {
		dossier next_dossier = dossier::from_file(p.path().string(), key);
		dossier_map.insert(pair<int, dossier>(next_dossier.get_idx(), next_dossier));
	}
}

void dossier_manager::update_state() {
	string targetFilename = dir;
	targetFilename.append("\\");
	targetFilename.append(current_dossier.get_filename());
	
	current_dossier.to_file(targetFilename, key);
	//Since current_dossier is an object copy, we need to update the master
	dossier_map[current_dossier.get_idx()] = current_dossier;
}

void dossier_manager::ui_loop(istream& in, ostream& out) {
	bool stay_alive = true;
	while (stay_alive) {
		string line;
		getline(in, line);
		if (line.find("quit") != string::npos) {
			stay_alive = false;
		}
		else if (line.find("ld") != string::npos || line.find("listdossiers") != string::npos) {
			map<int, dossier>::iterator itr;
			for (itr = dossier_map.begin(); itr != dossier_map.end(); ++itr) {
				out << itr->first << " : "
					<< itr->second.get_first_name() << ' ' << itr->second.get_last_name();
				out << LINE_RETURN;
			}
		}
		else if (line.find("od") != string::npos || line.find("opendossier") != string::npos) {
			vector<jstring> elements = jstring(line.c_str()).split(" ");
			if (elements.size() < 2) {
				out << "opendossier <dossier number>" << endl;
				continue;
			}

			int i = std::stoi(elements.at(1).c_str());
			if (dossier_map.find(i) != dossier_map.end()) {
				current_dossier = dossier_map.at(i);
				active_dossier = true;
				out << current_dossier.to_string() << endl;
			}
			else {
				out << "Invalid idx supplied" << endl;
			}
			
		}
		else if (line.find("ufn") != string::npos || line.find("updatefirstname") != string::npos) {
			vector<jstring> elements = jstring(line.c_str()).split(" ");
			if (elements.size() < 2) {
				out << "updatefirstname <new first name>" << endl;
				continue;
			}
			if (active_dossier) {
				current_dossier.update_first_name(elements.at(1).c_str());
				update_state();
			}
			else {
				out << "Need to choose a dossier to open first" << endl;
			}
			
		}
		else if (line.find("umn") != string::npos || line.find("updatemiddlename") != string::npos) {
			vector<jstring> elements = jstring(line.c_str()).split(" ");
			if (elements.size() < 2) {
				out << "updatemiddlename <new middle name(s)>" << endl;
				continue;
			}
			if (active_dossier) {
				current_dossier.update_middle_names(elements.at(1).c_str());
				update_state();
			}
			else {
				out << "Need to choose a dossier to open first" << endl;
			}
			
		}
		else if (line.find("uln") != string::npos || line.find("updatelastname") != string::npos) {
			vector<jstring> elements = jstring(line.c_str()).split(" ");
			if (elements.size() < 2) {
				out << "updatelastname <new last name>" << endl;
				continue;
			}
			if (active_dossier) {
				current_dossier.update_last_name(elements.at(1).c_str());
				update_state();
			}
			else {
				out << "Need to choose a dossier to open first" << endl;
			}
		}
		else if (line.find("udob") != string::npos || line.find("updatedob") != string::npos) {
			vector<jstring> elements = jstring(line.c_str()).split(" ");
			if (elements.size() < 2) {
				out << "updatedob <new dob>" << endl;
				continue;
			}
			if (active_dossier) {
				current_dossier.update_dob(elements.at(1).c_str());
				update_state();
			}
			else {
				out << "Need to choose a dossier to open first" << endl;
			}
			
		}
		else if (line.find("ainf") != string::npos || line.find("addinformation") != string::npos) {
			if (!active_dossier) {
				out << "Need to choose a dossier to open first" << endl;
				continue;
			}
			out << "Enter additional info. '<done>' to complete entry." << endl;
			getline(in, line);
			std::stringstream info;
			while (line.find("<done>") == string::npos) {
				info << line << LINE_RETURN;
				getline(in, line);
			}
			current_dossier.append_information(info.str());

			update_state();
		}
		else if (line.find("add") != string::npos || line.find("adddossier") != string::npos) {
			dossier new_dossier;
			out << "First name?: " << endl;
			getline(in, line);
			new_dossier.update_first_name(line);
			string firstname = line;
			out << "Middle name?: " << endl;
			getline(in, line);
			new_dossier.update_middle_names(line);
			out << "Last name?: " << endl;
			getline(in, line);
			new_dossier.update_last_name(line);
			out << "DOB?: " << endl;
			getline(in, line);
			new_dossier.update_dob(line);
			out << "Enter information. '<done>' to quit" << endl;
			getline(in, line);
			std::stringstream info;
			while (line.find("<done>") == string::npos) {
				info << line << LINE_RETURN;
				getline(in, line);
			}
			new_dossier.set_information(info.str());

			int test_idx = 1;
			bool continueChecking = true;
			while (continueChecking) {
				if (dossier_map.find(test_idx) == dossier_map.end()) {
					//We found an idx that isn't currently in use.
					continueChecking = false;
				}
				else {
					test_idx++;
				}
			}

			new_dossier.set_idx(test_idx);
			string targetFilename = dir;
			targetFilename.append("\\");
			targetFilename.append(new_dossier.get_filename());
			new_dossier.to_file(targetFilename, key);
			dossier_map.insert(pair<int, dossier>(test_idx, new_dossier));
		}
		else if (line.find("exd") != string::npos || line.find("exportdossier") != string::npos) {
			out << "WARNING: This will write dossier unencrypted to disk. Do you wish to continue? Y/N" << endl;
			getline(in, line);
			if (line.find("y") != string::npos || line.find("Y") != string::npos) {
				if (!active_dossier) {
					out << "Need to choose a dossier to open first" << endl;
				}
				else {
					string targetFilename = dir;
					targetFilename.append("\\");
					targetFilename.append(TMP_FILENAME);
					current_dossier.to_file(targetFilename, NULL);
				}
			}
		}
		else if (line.find("imp") != string::npos || line.find("importdossier") != string::npos) {
			out << "WARNING: This will overwrite current dossier. Do you wish to continue? Y/N" << endl;
			getline(in, line);
			if (line.find("y") != string::npos || line.find("Y") != string::npos) {
				string targetFilename = dir;
				targetFilename.append("\\");
				targetFilename.append(TMP_FILENAME);
				if (!active_dossier) {
					out << "Need to choose a dossier to open first" << endl;
				}
				else if (!std::filesystem::exists(targetFilename)) {
					out << "Program cannot find an unencrypted tempoary dossier at this location: " << targetFilename << endl;
				}
				else {
					dossier new_dossier = dossier::from_file(targetFilename);
					current_dossier.set_information(new_dossier.get_information());
					current_dossier.update_first_name(new_dossier.get_first_name());
					current_dossier.update_middle_names(new_dossier.get_middle_names());
					current_dossier.update_last_name(new_dossier.get_last_name());
					current_dossier.update_dob(new_dossier.get_dob());
					update_state();
					std::filesystem::remove(targetFilename);
				}
			}
		}
		else if (line.find("quit") != string::npos) {
			return;
		}
		else {
			out << "ld/listdossiers: list all available dossiers" << endl;
			out << "od/opendossier <#>: opens a dossier for editting" << endl;
			out << "ufn/updatefirstname <name>: updates first name" << endl;
			out << "ufn/updatemiddlename <name>: updates middle name" << endl;
			out << "uln/updatelastname <name>: updates last name" << endl;
			out << "udob/updatedob <dob>: updates date of birth" << endl;
			out << "ainf/addinformation: add information" << endl;
			out << "add/adddossier: launches the add dossier wizard" << endl;
			out << "exd/exportdossier: Exports the dossier to file system for manual edits" << endl;
			out << "imp/importdossier: Imports dossier back from filesystem" << endl;
		}
	}
}