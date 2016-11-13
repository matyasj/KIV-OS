#include "ps.h"
#include <iostream>


/*
Parametry:
0   Vypise informace o aktualnich procesech
*/
size_t __stdcall ps(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	Command* com = (Command*)regs.Rdx;

	std::string result;
	print_ps(&result);
	std::cout << result;
	//Write_File(output, result.c_str(), 0, written);
	return 0;
}
