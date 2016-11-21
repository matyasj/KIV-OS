#include "md.h"
#include"rtl_error.h"


/*
Parametry:
1   Vytvori novy adresar se zadanym jmenem / na zadane ceste
*/
size_t __stdcall md(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	int write_flag = (int)regs.Rsi;
	size_t written;
	if (!arg.empty()) {
		bool succes = Create_Folder(id,arg, 0);
		if (!succes){ 
			Write_File(id,error, print_error(Get_Last_Error()).c_str(), 0, written);
		}
	}
	else {
		Write_File(id,error, print_error(wrongArgument).c_str(), 0, written);
	}
	return 0;
}
