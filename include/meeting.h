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
			void to_file(string filename, uint8_t* key = NULL);
			static meeting* from_file(string filename, uint8_t* key = NULL);

			inline void add_attendee(string name) {
				attendees.push_back(name);
			}
			inline list<string> get_attendees() {
				return attendees;
			}

			inline void add_element(meeting_element* element) {
				elements.push_back(element);
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

			static meeting* meeting::from_file(istream* input, meeting* meeting);
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