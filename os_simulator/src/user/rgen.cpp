#include "rgen.h"
#include <iostream>


/*
Parametry:
 0   bude vypisovat náhodně vygenerovaná čísla v plovoucí čárce na stdout, dokud mu nepřijde znak Ctrl+Z //EOF
*/
std::string Rgen::run(void) {

	// TESTOVANI BUFFERU

	std::string output = get_whole_input();

	output += "rgen";

	save_whole_output(output);

	return output;
}
