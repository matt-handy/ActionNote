#pragma once

#include "meeting_element.h";
#include <list>;
#include <string>;

using namespace std;

namespace handy{
	namespace action_note{

		class meeting;

		const string ACTION_TAG = "@@AI ";
		const string ACTION_START_TAG = "@@start ";
		const string ACTION_DUE_TAG = "@@due ";
		const string ACTION_NOTE_TAG = "@@note ";
		const string ACTION_CLOSED_TAG = "@@closed ";
		const string ACTION_END_TAG = "@@endAI";

		class action_item : virtual public meeting_element {
		private:
			string assignee;
			string due_date;
			string start_date;
			string action_text;
			string closed_date;
			list<string> notes;
			meeting* parent;

		public:
			action_item(string a_assignee, string a_start_date, string a_due_date, string a_action_text, string a_closed_date, meeting* a_parent) {
				assignee = a_assignee;
				due_date = a_due_date;
				start_date = a_start_date;
				action_text = a_action_text;
				closed_date = a_closed_date;
				parent = a_parent;
			}

			action_item(const action_item& other) {
				assignee = other.assignee;
				due_date = other.due_date;
				start_date = other.due_date;
				action_text = other.due_date;
				closed_date = other.closed_date;
				notes = other.notes;
				parent = other.parent;
			}

			virtual string render_text();

			inline string get_assignee() {
				return assignee;
			}

			inline string get_due_date() {
				return due_date;
			}

			inline string get_start_date() {
				return start_date;
			}

			inline string get_action_text() {
				return action_text;
			}

			inline list<string> get_notes(){
				return notes;
			}

			inline string get_closed_date() {
				return closed_date;
			}

			inline void add_note(string new_note) {
				notes.push_back(new_note);
			}

			inline meeting* get_parent_meeting() {
				return parent;
			}

			inline bool remove_note(int idx) {
				if (idx < notes.size()) {
					std::list<string>::iterator ptr = notes.begin();
					advance(ptr, idx);
					notes.erase(ptr);
				}
				else {
					return false;
				}
			}

		};

	}
}
