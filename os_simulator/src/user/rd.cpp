#include "rd.h"


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
	std::string arg = (char *)regs.Rdx;
	if (!arg.empty()) {
		bool success = Delete_Folder(arg, 0);
		if (success) {
			return 0;
		}
		else {
			//TODO chyba
		}
	}
	else {
		//TODO chyba
	}
	return 0;
}
