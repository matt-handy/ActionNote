#pragma once
#include <list>
#include <string>
#include "meeting_element.h"
#include "action_item.h"

using namespace std;

namespace handy{
	namespace action_note {
		const string ATTENDEE = "Attendees: ";

		class meeting {
		public: 
			void to_file(string filename);
			static meeting* from_file(string filename);
			inline list<string> get_attendees() {
				return attendees;
			}
			inline list<meeting_element*> get_elements() {
				return elements;
			}
			inline list<action_item*> get_actions() {
				return actions;
			}
			inline string get_original_file() {
				return original_file;
			}

		private:
			list<string> attendees;
			list<meeting_element *> elements;
			list<action_item*> actions;
			string original_file;
		};

		class meeting_text : virtual public meeting_element {
		private:
			string content;
		public:
			meeting_text(string text) {
				content = text;
			}

			string render_text() {
				return content;
			}
		};
	}
}