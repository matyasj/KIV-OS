#include "md.h"


/*
Parametry:
1   Vytvori novy adresar se zadanym jmenem / na zadane ceste
*/
void Md::run(void) {
	
	std::string output = "";
	std::string name = "";

	if(_arguments.size()!=0) name = _arguments.at(0);
	//int _thread_id = Create_Thread(MD,-1,&name);
	bool succes = Create_Folder(name,0);
	//Execute_Thread(_thread_id);
	if (succes) {
		output = "Adresar vytvoren";
	}
	else {
		_error = true;
		output = "Chyba";
	}

	save_whole_output(output, true);
}
