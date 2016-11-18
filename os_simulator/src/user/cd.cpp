#include "cd.h"



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
	std::string arg = (char *)regs.Rdx;
	size_t written;
	std::string buffer;
	if (arg.empty()) {
		bool success = Print_Folder(id, arg.c_str(), &buffer);
		Write_File(output, buffer.c_str(), 0, written);
	}
	else {

	}
}
