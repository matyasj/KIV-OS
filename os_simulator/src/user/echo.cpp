#include "echo.h"
#include"rtl_error.h"
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
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	std::vector<std::string> lines;
	size_t written;
	//if (!arg.empty()) {
		std::string arg_str = arg;
		std::stringstream str;
		str << arg_str << std::endl;
		std::string strr = str.str();
		Write_File(output, strr.c_str(), 2, written);
	//}
	/*else {
		Write_File(error, print_error(onlyOn).c_str(), 0, written);
	}*/
	return 0;
}

