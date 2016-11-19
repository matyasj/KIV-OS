#include "cd.h"
#include"rtl_error.h"



/*
Parametry:
0   Zobrazi aktualni adresar a jeho cestu
1   Zmeni pracovni adresar dle zadane cesty

Prepinace
/D  Zmeni aktualni jednotku (to se nas asi netyka)

*/
size_t __stdcall cd(const CONTEXT &regs){
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	size_t written;
	std::string buffer;
	if (arg.empty()) {
		bool success = Print_Folder(id, arg.c_str(), &buffer);
		Write_File(output, buffer.c_str(), 0, written);
	}
	else {
		bool success = Change_Folder(id, arg.c_str());
		if (!success) {
			Write_File(error, print_error(Get_Last_Error()).c_str(), 0, written);
		}
	}
	return 0;
}
