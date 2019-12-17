#include "main.h"

#include <iostream>
#include <string>

using namespace handy::action_note;
using namespace std;


int main() {
	runner my_main;
	my_main.loop(cin, cout, "thisdir");
}
