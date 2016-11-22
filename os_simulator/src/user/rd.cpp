#include "rd.h"
#include"rtl_error.h"

/*
Parametry:
1   Smaze dany adresar (na dane ceste)
*/

size_t __stdcall rd(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	int write_flag = (int)regs.Rsi;
	size_t written;
	if (!arg.empty()) {
		bool success = Delete_Folder(id,arg, 0);
		if (!success) {
			Write_File(id,error, print_error(Get_Last_Error()).c_str(), 0, written);
		}
	}
	else {
		Write_File(id,error, print_error(wrongArgument).c_str(), 0, written);
	}
	return 0;
}
