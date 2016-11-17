#include "echo.h"
#include <iostream>

/*
Parametry:
0	Vypise jestli je ECHO rezim zapnut (nevim jestli budem delat)
1	ON / OFF - Vypne/Zapne zobrazovani zprav (ECHO rezim)
1   Vypise zadanou zpravu

*/
size_t __stdcall echo(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	Command* com = (Command*)regs.Rdx;

	std::vector<std::string> lines;
	if (com->has_argument) {
		std::string argument = com->arguments.at(0);
		size_t written;
		Write_File(output, argument.c_str(), 0, written);
	}
	else {
		// TODO - chyba
		// error - > echo je pouze out
	}
	return 0;
}

