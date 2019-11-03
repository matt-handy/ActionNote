#pragma once
#include <string>

using namespace std;

namespace handy {
	namespace action_note {
		class meeting_element {
		public:
			virtual string render_text() = 0;
		};
	}
}