#pragma once
#include <string>

using namespace std;

namespace handy {
	namespace action_note {
		const string LINE_RETURN = "\n";

		class meeting_element {

		public:
			virtual string render_text() = 0;
		};
	}
}