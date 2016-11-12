#include "rgen.h"


/*
Parametry:
0   bude vypisovat náhodně vygenerovaná čísla v plovoucí čárce na stdout, dokud mu nepřijde znak Ctrl+Z //EOF
*/
size_t __stdcall rgen(const CONTEXT &regs) {

	int min = 0;
	int max = 9;

	double number = 0.0;
	std::string str = "";

	for (int i = 0; i < 10; i++) {

		number = ((double)rand() / (double)RAND_MAX);
		str = std::to_string(number);

		std::cout << str << std::endl;
	}

	return 0;
}