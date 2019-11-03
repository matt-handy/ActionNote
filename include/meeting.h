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
		private:
			list<string> attendees;
			list<meeting_element *> elements;
		};
	}
}