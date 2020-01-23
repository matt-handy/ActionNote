#include "main.h"

#include <iostream>
#include <string>

using namespace handy::action_note;
using namespace std;


int main(int argc, char* argv[]) {
	runner my_main;
	if (argc == 1) {
		my_main.loop(cin, cout, "thisdir");
	}
	else if (argc = 2) {
		my_main.loop(cin, cout, "thisdir", (uint8_t*)argv[1]);
	}
	else {
		cout << "Improper argument set" << endl;
	}
}
