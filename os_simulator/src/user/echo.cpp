#include "echo.h"
#include"rtl_error.h"
#include <iostream>
#include<sstream>

/*
Parametry:
   Vypise zadanou zpravu
   Musi mit argumet
*/
size_t __stdcall echo(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	int write_flag = (int)regs.Rsi;
	std::vector<std::string> lines;
	size_t written;
	if (!arg.empty()) {
		std::string arg_str = arg;
		std::stringstream str;
		str << arg_str << std::endl;
		std::string strr = str.str();
		Write_File(id,output, strr.c_str(), write_flag, written);
	}
	else {
		Write_File(id,error, print_error(onlyOn).c_str(), 0, written);
	}
	return 0;
}

