#include "action_item.h"
#include "meeting_manager.h"
#include "jstring.h"

#include <iostream>
#include <string>

using namespace handy::action_note;
using namespace std;

int main() {
	bool stay_alive = true;
	meeting_manager my_man("thisdir");
	my_man.initialize();

	while (stay_alive) {
		string line; 
		getline(cin, line);
		if (line.find("quit") != string::npos) {
			stay_alive = false;
		}
		else if (line.find("oas") != string::npos || line.find("openactions") != string::npos) {
			vector<action_item*> open_actions = my_man.get_open_actions();
			for (int idx = 0; idx < open_actions.size(); idx++) {
				action_item* current = open_actions.at(idx);
				cout << "Idx: " << idx << current->render_text() << endl;
			}
		}
		else if (line.find("ca") != string::npos || line.find("closeaction") != string::npos) {
			vector<action_item*> open_actions = my_man.get_open_actions();
			vector<jstring> elements = jstring(line.c_str()).split(" ");
			if (elements.size() < 2) {
				cout << "closeaction <action_index> <optional comment>" << endl;
				continue;
			}

			try {
				int i = std::stoi(elements.at(1).c_str());
				if (i >= open_actions.size() || i < 0) {
					cout << "Index not in range" << endl;
					continue;
				}
				action_item* target_ai = open_actions.at(i);
				
				if (elements.size() > 2) {
					string close_msg = "Target AI Closed";
					close_msg.append(": ");
					close_msg.append(line.substr(elements.at(0).len() + elements.at(1).len() + 1));
					target_ai->add_note(close_msg);
				}

				target_ai->close_action();
				
				my_man.save_action(target_ai);
			}
			catch (...) {
				cout << "Must supply valid integer" << endl;
				continue;
			}
		}
		else if (line.find("qa") != string::npos || line.find("quickaction") != string::npos) {
			string due_date;
			string start_date;
			string text;
			string mnemonic;
			cout << "Mnemonic?" << endl;
			getline(cin, line);
			if (line.size() != 0) {
				mnemonic = line;
			}
			else {
				cout << "Mnemonic required, moron." << endl;
				continue;
			}

			cout << "Due Date?" << endl;
			getline(cin, line);
			if (line.size() != 0) {
				if (line.find("cancel") != string::npos) {
					continue;
				}
				due_date = line;
			}

			cout << "Start Date?" << endl;
			getline(cin, line);
			if (line.size() != 0) {
				if (line.find("cancel") != string::npos) {
					continue;
				}
				start_date = line;
			}

			cout << "Enter the action" << endl;
			bool in_text = true;
			while (in_text) {
				getline(cin, line);
				if (line.size() != 0) {
					if (line.find("done") != string::npos) {
						in_text = false;
					}
					else {
						if (text.size() != 0) {
							text.append(LINE_RETURN);
						}
						text.append(line);
					}
					
				}
			}
			my_man.create_action_note(mnemonic ,"myself", due_date, start_date, text);
		}
		else if (line.find("addnote") != string::npos) {
			vector<action_item*> open_actions = my_man.get_open_actions();
			vector<jstring> elements = jstring(line.c_str()).split(" ");

			if (elements.size() < 2) {
				cout << "closeaction <action_index> <optional comment>" << endl;
				continue;
			}

			try {
				int i = std::stoi(elements.at(1).c_str());
				if (i >= open_actions.size() || i < 0) {
					cout << "Index not in range" << endl;
					continue;
				}
				action_item* target_ai = open_actions.at(i);

				cout << "Enter your note: " << endl;
				getline(cin, line);
				if (line.size() != 0) {
					if (line.find("cancel") != string::npos) {
						continue;
					}
					my_man.addActionNote(line, i);
				}
			}
			catch (...) {
				cout << "Must supply valid integer" << endl;
				continue;
			}
		}
		else if (line.find("quit") != string::npos) {
			stay_alive = false;
		}
	}
}