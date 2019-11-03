#include "meeting.h"
#include "action_item.h"
#include <iostream>
#include <fstream>
#include <string>
#include <jstring.h>

using namespace handy;
using namespace handy::action_note;

meeting meeting::from_file(string filename) {
	meeting meeting;
	string line;
	ifstream myfile(filename);
	string current_segment;
	
	if (myfile.is_open())
	{
		bool in_action = false;
		while (getline(myfile, line))
		{
			if (line.find(ATTENDEE) != std::string::npos) {
				string attendee_list = line.substr(ATTENDEE.size());
				jstring str = jstring((char *)attendee_list.c_str());
				jstring splitter = jstring((char*)", ");
				std::vector<jstring> attendee_v = str.split(splitter);
				for (vector <jstring> ::iterator it = attendee_v.begin(); it != attendee_v.end(); ++it) {
					meeting.attendees.push_back(it->c_str());
				}
			}
			else if (in_action) {
				//Action fields in first line
				//Action text in second+
				//Action notes after first note tag, reset with each tag
				//end action with final tag
			}
			else if (line.find(ACTION_TAG) != std::string::npos) {
				
			}
			else {
				current_segment.append(line);
				current_segment.append("\r\n");
			}
		}
		myfile.close();
	}

	else cout << "Unable to open file";

	return meeting;
}