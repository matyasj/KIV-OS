#include "dir.h"
#include"rtl_error.h"

size_t __stdcall dir(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	size_t written;
	std::string buffer;
	bool success = Print_Folder(id,arg.c_str() ,&buffer);
	if (success) {
		Write_File(output, buffer.c_str(), 0, written);
	}
	else {
		Write_File(error, print_error(Get_Last_Error()).c_str(), 0, written);
	}
	return 0;
}
