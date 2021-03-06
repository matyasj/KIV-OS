#include "dir.h"
#include"rtl_error.h"

/*
Vypise strukturu adresare
v argumentech muze byt cesta.
	- pokud neni - vypise pracovni adresar	
*/
size_t __stdcall dir(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	int write_flag = (int)regs.Rsi;
	std::string arg = (char *)regs.Rdx;
	size_t written;
	std::string buffer;
	bool success = Print_Folder(id,arg.c_str() ,&buffer);
	if (success) {
		Write_File(id,output, buffer.c_str(), write_flag, written);
	}
	else {
		Write_File(id,error, print_error(Get_Last_Error()).c_str(), 0, written);
	}
	return 0;
}
