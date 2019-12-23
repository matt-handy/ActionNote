#pragma once

#include "action_item.h"
#include "meeting_manager.h"
#include "jstring.h"
#include "timeutil.h";
#include <list>;
#include <string>;



using namespace std;

namespace handy{
	namespace action_note{

		

		class runner{
		private:
			

		public:
			void loop(istream &in, ostream& out, string dir);

			bool pull_action(string& actionee, string& mnemonic, string& due_date, string& start_date, string& text, istream &in, ostream &out);
		};

	}
}
