#include "dir.h"

size_t __stdcall dir(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	std::string arg = (char *)regs.Rdx;
	size_t written;
	std::string buffer;
	bool success = Print_Folder(id,arg.c_str() ,&buffer);
	Write_File(output, buffer.c_str(), 0, written);
	return 0;
}
