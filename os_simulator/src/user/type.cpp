#include "type.h"
#include"rtl_error.h"
#include<string>
#include <iostream>

/*
void create(std::string name) {
	THandle fileHelp = Create_File(name.c_str(), FILE_WRITE_ACCESS);
	size_t written;
	std::string str = "prvni soubor";
	Write_File(fileHelp, str.c_str(), 0, written);
	Close_File(fileHelp);
}*/

/*
Parametry:
1   Zobrazi obsah textoveho souboru
rdx Command
rbx input Handler
rcx output Handler
rax error
rdi  id thread
*/

size_t __stdcall type(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle error = (THandle)regs.Rax;
	THandle output = (THandle)regs.Rcx;
	std::string arg = (char *)regs.Rdx;
	size_t written;
	if (!arg.empty()) {
		std::string argument = arg;
		THandle file = Open_File(id,argument.c_str(), FILE_SHARE_READ);
		std::string buffer;
		size_t read;
		if (Get_Last_Error() == 0)
		{
			bool succes = Read_File(id,file, &buffer, 0, read);
			Close_File(id,file);
			if (!succes) {
				Write_File(id,error, print_error(Get_Last_Error()).c_str(), 0, written);
			}
			else {
				if (read > 0) buffer.erase(read - 1);	//odstranit \0
				Write_File(id,output, buffer.c_str(), 0, written);
			}
		}
		else {
			Write_File(id,error, print_error(Get_Last_Error()).c_str(), 0, written);
		}
	}
	else {
		// chyba - ma argument
		Write_File(id,error, print_error(wrongArgument).c_str(), 0, written);
	}
	return 0;
}
