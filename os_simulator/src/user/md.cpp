#include "md.h"


/*
Parametry:
1   Vytvori novy adresar se zadanym jmenem / na zadane ceste
*/
size_t __stdcall md(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	std::string arg = (char *)regs.Rdx;
	if (!arg.empty()) {
		bool succes = Create_Folder(arg, 0);
		if (succes) {
			return 0;
		}
		else {
			//TODO chyba
		}
	}
	else {
		//TODO - indikovat chybu
	}
	return 0;
}
