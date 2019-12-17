#include "meeting.h"
#include "action_item.h"
#include <iostream>
#include <fstream>
#include <string>
#include <jstring.h>

using namespace handy;
using namespace handy::action_note;

void meeting::to_file(string filename) {
	original_file = filename;
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
			else if (line.find(ACTION_TAG) != std::string::npos) {
#ifdef DEBUG
				cout << "Found action tag: " << line << endl;
#endif
				if (current_segment.size() != 0) {
					meeting->elements.push_back(new meeting_text(current_segment));
					current_segment = "";
				}

				action_item* ai = action_item::get_action_item(line, &myfile, meeting);
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