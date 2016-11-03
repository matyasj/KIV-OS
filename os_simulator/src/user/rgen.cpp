#include "rgen.h"
#include <iostream>


/*
Parametry:
 0   bude vypisovat náhodně vygenerovaná čísla v plovoucí čárce na stdout, dokud mu nepřijde znak Ctrl+Z //EOF
*/
std::string Rgen::run(void) {
	
	int min = 0;
	int max = 9;

	int number = 0;
	std::string str = "";

	for (int i = 0; i < 10; i++) {

		number = min + (rand() % (int)(max - min + 1));
		str = std::to_string(number);

		_bout->push(str);
	}

	_bout->close();

	return "";
}
