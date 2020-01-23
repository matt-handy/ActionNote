#include "meeting_manager.h"
#include "timeutil.h"
#include <iostream>
#include <fstream>
#include <string>
#include "jstring.h"
#include <filesystem>
#include <time.h>

using namespace std;
using namespace handy::action_note;
using namespace handy::common;
namespace fs = std::filesystem;

bool meeting_manager::saveas_meeting(string meeting_name, meeting* meeting) {
	string filename = dir;
	filename.append(PATH_SEP);
	string timestamp = timeutil::get_YYYYMMDD_current();
	filename.append(meeting_name);
	filename.append(timestamp);
	filename.append(".mt");
	meeting->to_file(filename, e_key);

	meetings.push_back(meeting);

	return true;
}

void meeting_manager::initialize() {
	for (auto& p : fs::directory_iterator(dir)){
		//Assume that only files with extensions are valid meetings
		if (p.path().extension().generic_string().find("mt") != string::npos) {
#ifdef DEBUG
			cout << p.path().string() << endl;
#endif
			meeting* read_meeting = meeting::from_file(p.path().string(), e_key);
#ifdef DEBUG
			cout << read_meeting << endl;
#endif
			meetings.push_back(read_meeting);
		}
		else if (p.path().extension().generic_string().find("note") != string::npos) {
			action_item* ai = action_item::get_action_item(p.path().string(), e_key);
			free_actions.push_back(ai);
		}
	}
#ifdef DEBUG
	cout << "Ingest complete" << endl;
#endif
}

vector<action_item*> meeting_manager::get_open_actions() {
	vector<action_item*> actions;

	for (int idx = 0; idx < free_actions.size(); idx++) {
		action_item* ai = free_actions.at(idx);
		if (ai->get_closed_date().size() == 0) {
			actions.push_back(ai);
		}
	}

	for (int idx = 0; idx < meetings.size(); idx++) {
		meeting* meeting = meetings.at(idx);
#ifdef DEBUG
		cout << meeting << endl;
#endif
		list<action_item*> ais = meeting->get_actions();
		list<action_item*>::iterator elements = ais.begin();
#ifdef DEBUG
		cout << "Examining actions: " << *elements << endl;
#endif

		//TODO: HAAAAAAAAAAXXXXXXXXXXX - use a real check on the iterator instead of
		//this duplicative crap.
		int jdx = 0;
		while (jdx < ais.size()) {
#ifdef DEBUG
			cout << (*elements)->render_text() << endl;
#endif
			if ((*elements)->get_closed_date().size() == 0) {
				actions.push_back(*elements);
#ifdef DEBUG
				cout << "Adding!" << endl;
#endif
			}
			
			jdx++;
			advance(elements, 1);
		}
		
	}

	return actions;
}

void meeting_manager::save_meeting(meeting* meeting) {
	meeting->to_file(meeting->get_original_file(), e_key);
}

bool meeting_manager::save_action(action_item* action) {
	if (action != NULL && action->get_parent_meeting()) {
		save_meeting(action->get_parent_meeting());
		return true;
	}
	else {
		return action->save();
	}
}

bool meeting_manager::addActionNote(string note, int openActionIdx) {
	if (openActionIdx > get_open_actions().size()) {
		return false;
	}

	get_open_actions().at(openActionIdx)->add_note(note);
	return save_action(get_open_actions().at(openActionIdx));
}

void meeting_manager::create_action_note(string mnemonic, string actionee, string due_date, string start_date, string note) {
	
	action_item* ai = new action_item(actionee, start_date, due_date, note, string(), NULL);
	string newFileName = dir;
	newFileName.append(PATH_SEP);
	newFileName.append(mnemonic);
	newFileName.append(timeutil::get_YYYYMMDD_current());
	newFileName.append(".note");
	
#ifdef DEBUG
	if(e_key){
		cout << "Saving action as: " << newFileName << "with key: " << e_key << endl;
	}else{
		cout << "Saving action as: " << newFileName << endl;
	}
#endif
	ai->save_as(newFileName, e_key);
	free_actions.push_back(ai);
}