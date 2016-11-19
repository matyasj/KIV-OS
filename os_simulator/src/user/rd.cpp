#include "rd.h"
#include"rtl_error.h"

/*
Parametry:
1   Smaze dany adresar (na dane ceste)

Nevim jak to udelame bez prepinacu to nesmaze plny adresar
/S - smaze vcetne podaresaru
/Q - smaze bez potvrzovani

Asi bych to udelal ze rovnou smaze vse - ale muzem se zeptat
*/

size_t __stdcall rd(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
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
