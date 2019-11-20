#include "action_item.h"

#include <sstream>

using namespace std;
using namespace handy::action_note;

string action_item::render_text() {
	stringstream output;

	output << ACTION_TAG << assignee;
	if (start_date.size() != 0) {
		output << " " << ACTION_START_TAG << start_date;
	}
	if (due_date.size() != 0) {
		output << " " << ACTION_DUE_TAG << due_date;
	}
	output << LINE_RETURN;

	output << action_text << LINE_RETURN;

	if (notes.size() != 0) {
		for (list<string>::const_iterator i = notes.begin(); i != notes.end(); ++i)
		{
			output <<ACTION_NOTE_TAG << i->c_str() << LINE_RETURN;
		}
	}

	if (closed_date.size() != 0) {
		output << ACTION_CLOSED_TAG << closed_date << LINE_RETURN;
	}

	output << ACTION_END_TAG << LINE_RETURN;

	return output.str();
}