#pragma once
#include <time.h>

namespace handy {
	namespace action_note {
		class timeutil {
		public:
			static string get_YYYYMMDD_current() {
				time_t rawtime;
				struct tm* timeinfo;
				char buffer[80];
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				strftime(buffer, 80, "%Y-%m-%d", timeinfo);
				return buffer;
			}
		};
	}
}