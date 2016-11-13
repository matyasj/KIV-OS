#include "md.h"


/*
Parametry:
1   Vytvori novy adresar se zadanym jmenem / na zadane ceste
*/
size_t __stdcall md(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	Command* com = (Command*)regs.Rdx;

	std::vector<std::string> lines;
	if (com->has_argument) {
		std::string argument = com->arguments.at(0);
		bool succes = Create_Folder(argument, 0);
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
}
