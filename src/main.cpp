#include "action_item.h"

using namespace handy::action_note;

int main() {
	action_item test_ai = action_item("myself", "12/14/2019", "12/15/2019", "DO ALL OF THE THINGS", "", NULL);
	test_ai.add_note("12/13/2019 - I prepared to do the thing");
	test_ai.add_note("12/14/2019 - I started to do the thing");

	list<string> notes = test_ai.get_notes();
	
	list<string>::iterator notes_itr = notes.begin();
	notes_itr++;
}