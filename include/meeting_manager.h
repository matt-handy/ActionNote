#pragma once
#include "meeting.h"
#include "action_item.h"
#include <vector>

using namespace std;

namespace handy {
	namespace action_note {
		class meeting_manager {
		public:
			meeting_manager(string directory) {
				dir = directory;
			}

			~meeting_manager() {
				//TODO: delete all meetings
			}

			//Loads a set of all meetings and notes into memory
			void initialize();

			//Gets all open action items from meetings in memory
			vector<action_item*> get_open_actions();

			//Saves meeting changes
			void save_meeting(meeting* meeting);

			//Saves action item changes
			bool save_action(action_item* action_item);

			bool addActionNote(string note, int openActionIdx);
		private:
			string dir;

			vector<meeting*> meetings;
			vector<action_item*> free_actions;

		};
	}
}