#include "main.h"

#include <iostream>
#include <string>

using namespace handy::common;
using namespace handy::action_note;
using namespace std;

bool runner::pull_action(string& actionee, string& mnemonic, string& due_date, string& start_date, string& text, istream &in, ostream &out) {
	string line;
	out << "Mnemonic?" << endl;
	getline(in, line);
	if (line.size() != 0) {
		mnemonic = line;
	}
	else {
		out << "Mnemonic required, moron." << endl;
		return false;
	}

	out << "Actionee? (default is myself)" << endl;
	getline(in, line);
	if (line.size() != 0) {
		if (line.find("cancel") != string::npos) {
			return false;
		}
		actionee = line;
	}
	else {
		actionee = "myself";
	}

	out << "Due Date?" << endl;
	getline(in, line);
	if (line.size() != 0) {
		if (line.find("cancel") != string::npos) {
			return false;
		}
		due_date = line;
	}

	out << "Start Date?" << endl;
	getline(in, line);
	if (line.size() != 0) {
		if (line.find("cancel") != string::npos) {
			return false;
		}
		start_date = line;
	}

	out << "Enter the action" << endl;
	bool in_text = true;
	while (in_text) {
		getline(in, line);
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
	return true;
}

void runner::loop(istream& in, ostream& out, string dir) {
	bool stay_alive = true;
	meeting_manager my_man(dir);
	my_man.initialize();

	while (stay_alive) {
		string line; 
		getline(in, line);
		if (line.find("quit") != string::npos) {
			stay_alive = false;
		}
		else if (line.find("oas") != string::npos || line.find("openactions") != string::npos) {
			vector<action_item*> open_actions = my_man.get_open_actions();
			for (int idx = 0; idx < open_actions.size(); idx++) {
				action_item* current = open_actions.at(idx);
				out << "Idx: " << idx << current->render_text() << endl;
			}
		}
		else if (line.find("ca") != string::npos || line.find("closeaction") != string::npos) {
			vector<action_item*> open_actions = my_man.get_open_actions();
			vector<jstring> elements = jstring(line.c_str()).split(" ");
			if (elements.size() < 2) {
				out << "closeaction <action_index> <optional comment>" << endl;
				continue;
			}

			try {
				int i = std::stoi(elements.at(1).c_str());
				if (i >= open_actions.size() || i < 0) {
					out << "Index not in range" << endl;
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
				out << "Must supply valid integer" << endl;
				continue;
			}
		}
		else if (line.find("qa") != string::npos || line.find("quickaction") != string::npos) {
			string due_date;
			string start_date;
			string text;
			string mnemonic;
			string actionee;
			
			if (pull_action(actionee, mnemonic, due_date, start_date, text, in, out)) {
				my_man.create_action_note(mnemonic, "myself", due_date, start_date, text);
			}
		}
		else if (line.find("minutes") != string::npos) {
			out << "Enter your minutes!" << endl;
			out << ATTENDEE_CMD <<" <Attendee Name> to add to attendee list" << endl;
			out << ACTION_START_CMD << " to enter action wizard" << endl;
			out << MEETING_SAVE_CMD << " <Meeting Name> to close out meeting" << endl;
			meeting *my_meeting = new meeting();
			bool in_meeting = true;
			string curr_text_block;
			while (in_meeting) {
				getline(in, line);
				if (line.find(ATTENDEE_CMD) != string::npos) {
					string attendee = line.substr(ATTENDEE_CMD.size() + 1);
					my_meeting->add_attendee(attendee);
				}
				else if (line.find(ACTION_START_CMD) != string::npos) {
					string due_date;
					string start_date;
					string text;
					string mnemonic;
					string actionee;

					if (pull_action(actionee, mnemonic, due_date, start_date, text, in, out)) {
						if (curr_text_block.size() != 0) {
							curr_text_block.append(LINE_RETURN);
							meeting_text* m_text = new meeting_text(curr_text_block);
							my_meeting->add_element(m_text);
							curr_text_block = "";
						}

						action_item* ai = new action_item("myself", start_date, due_date, text, "", my_meeting);
						my_meeting->add_element(ai);

						out << "Action saved" << endl;
					}
					
					
				}
				else if (line.find(MEETING_SAVE_CMD) != string::npos) {
					if (curr_text_block.size() > 0) {
						meeting_text* m_text = new meeting_text(curr_text_block);
						my_meeting->add_element(m_text);
					}
					string meeting_name = line.substr(MEETING_SAVE_CMD.size() + 1);
					my_man.saveas_meeting(meeting_name, my_meeting);
					out << "Meeting saved" << endl;
					in_meeting = false;
				}
				else {
					curr_text_block.append(LINE_RETURN);
					curr_text_block.append(line);
				}
			}
		}
		else if (line.find("addnote") != string::npos) {
			vector<action_item*> open_actions = my_man.get_open_actions();
			vector<jstring> elements = jstring(line.c_str()).split(" ");

			if (elements.size() < 2) {
				out << "closeaction <action_index> <optional comment>" << endl;
				continue;
			}

			try {
				int i = std::stoi(elements.at(1).c_str());
				if (i >= open_actions.size() || i < 0) {
					out << "Index not in range" << endl;
					continue;
				}
				action_item* target_ai = open_actions.at(i);

				out << "Enter your note: " << endl;
				getline(in, line);
				if (line.size() != 0) {
					if (line.find("cancel") != string::npos) {
						continue;
					}
					my_man.addActionNote(line, i);
				}
			}
			catch (...) {
				out << "Must supply valid integer" << endl;
				continue;
			}
		}
		else if (line.find("quit") != string::npos) {
			stay_alive = false;
		}
		else {
		out << "Unknown command: " << line << endl;
		}
	}
}

