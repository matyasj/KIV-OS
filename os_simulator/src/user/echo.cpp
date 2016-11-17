#include "echo.h"
#include <iostream>
#include<sstream>

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
	std::string arg = (char *)regs.Rdx;
	std::vector<std::string> lines;
	if (!arg.empty()) {
		std::string arg_str = arg;
		std::stringstream str;
		str << arg_str << std::endl;
		size_t written;
		std::string strr = str.str();
		Write_File(output, strr.c_str(), 2, written);
	}
	else {
		// TODO - chyba
		// error - > echo je pouze out
	}
	return 0;
}

