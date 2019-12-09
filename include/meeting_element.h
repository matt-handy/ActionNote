#pragma once
#include <string>

using namespace std;

namespace handy {
	namespace action_note {
		const string LINE_RETURN = "\n";
#ifdef _WIN32
		const string PATH_SEP = "\\";
#else
		const string PATH_SEP = "/";
#endif
		

		class meeting_element {

		public:
			virtual string render_text() = 0;
		};
	}
}