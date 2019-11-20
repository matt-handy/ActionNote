#include "meeting.h"
#include "action_item.h"
#include <iostream>
#include <fstream>
#include <string>
#include <jstring.h>

using namespace handy;
using namespace handy::action_note;

void meeting::to_file(string filename) {
	ofstream myfile;
	myfile.open(filename);
	
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

	myfile.close();
}

meeting* meeting::from_file(string filename) {
	meeting* meeting = new handy::action_note::meeting();
	meeting->original_file = filename;
	string line;
	ifstream myfile(filename);
	string current_segment;
	
	string curr_ai_assignee;
	string curr_ai_start;
	string curr_ai_due;
	string curr_ai_text;
	string curr_ai_enddate;
	vector<string> ai_notes;

	if (myfile.is_open())
	{
		bool in_action = false;
		bool in_action_text = false;
		while (getline(myfile, line))
		{
			if (line.find(ATTENDEE) != std::string::npos) {
#ifdef DEBUG
				cout << "Found attendees: " << line << endl;
#endif
				string attendee_list = line.substr(ATTENDEE.size());
				jstring str = jstring((char *)attendee_list.c_str());
				jstring splitter = jstring((char*)", ");
				std::vector<jstring> attendee_v = str.split(splitter);
				for (vector <jstring> ::iterator it = attendee_v.begin(); it != attendee_v.end(); ++it) {
					meeting->attendees.push_back(it->c_str());
				}
			}
			else if (in_action) {
				if (line.find(ACTION_CLOSED_TAG) != std::string::npos) {
#ifdef DEBUG
					cout << "Pushing closed tag!" << endl;
#endif
					curr_ai_enddate = line.substr(ACTION_CLOSED_TAG.size());
				}else if (line.find(ACTION_NOTE_TAG) != std::string::npos) {
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
					in_action = false;
					action_item* ai = new action_item(curr_ai_assignee, curr_ai_start, curr_ai_due, curr_ai_text, curr_ai_enddate, meeting);
#ifdef DEBUG
					cout << "AI: " << ai << endl;
#endif
					for (int idx = 0; idx < ai_notes.size(); idx++) {
						ai->add_note(ai_notes.at(idx));
					}
					meeting->elements.push_back(ai);
					meeting->actions.push_back(ai);

					curr_ai_assignee = "";
					curr_ai_due = "";
					curr_ai_start = "";
					curr_ai_enddate = "";
					curr_ai_text = "";
					ai_notes.clear();
				}
				else if (in_action_text) {
#ifdef DEBUG
					cout << "Line of action: " << line << endl;
#endif
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
			else if (line.find(ACTION_TAG) != std::string::npos) {
#ifdef DEBUG
				cout << "Found action tag: " << line << endl;
#endif
				if (current_segment.size() != 0) {
					meeting->elements.push_back(new meeting_text(current_segment));
					current_segment = "";
				}
				in_action = true;
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
				in_action_text = true;
			}
			else {
#ifdef DEBUG
				cout << "Appending to current segment: " << line << endl;
#endif
				current_segment.append(line);
				current_segment.append(LINE_RETURN);
			}
		}
		myfile.close();
		if (current_segment.size() != 0) {
			meeting->elements.push_back(new meeting_text(current_segment));
		}
	}

	else {
		//TODO: throw exception
		cout << "Unable to open file";
	}

	return meeting;
}