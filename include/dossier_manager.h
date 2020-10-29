#pragma once
#include "dossier.h"
#include <map>

namespace handy {
	namespace action_note {
		class dossier_manager {
		public:
			dossier_manager(string dir, uint8_t* key = NULL);
			
			void initialize();

			void ui_loop(istream& in, ostream& out);
		private:
			std::map<int, dossier> dossier_map;

			dossier current_dossier;
			bool active_dossier = false;

			void update_state();

			string dir;
			uint8_t* key;
		};
	}
}