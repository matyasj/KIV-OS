#include "md.h"


/*
Parametry:
1   Vytvori novy adresar se zadanym jmenem / na zadane ceste
*/
std::string Md::run(void)
{
	std::string name = "";
	if(_arguments.size()!=0) name = _arguments.at(0);
	bool succes = Create_Folder(name,0);
	if (succes) {
		return "Adresar vytvoren";
	}
	else {
		return "Chyba";
	}
}
