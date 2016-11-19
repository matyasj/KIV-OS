#include "ps.h"
#include"rtl_error.h"
#include <iostream>


/*
Parametry:
0   Vypise informace o aktualnich procesech
*/
size_t __stdcall ps(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	size_t written;
	if (!arg.empty()) {
		Write_File(error, print_error(wrongArgument).c_str(), 0, written);
	}
	else {
		std::string result;
		print_ps(&result);
		Write_File(output, result.c_str(), 0, written);
	}
	
	return 0;
}
