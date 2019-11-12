#pragma once
#include <list>
#include <string>
#include "meeting_element.h"

using namespace std;

namespace handy{
	namespace action_note {
		const string ATTENDEE = "Attendees: ";

		class meeting {
		public: 
			void to_file(string filename);
			static meeting from_file(string filename);
			inline list<string> get_attendees() {
				return attendees;
			}
			inline list<meeting_element*> get_elements() {
				return elements;
			}
		private:
			list<string> attendees;
			list<meeting_element *> elements;
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