#include "cd.h"
#include "Parser\instruction.h"


/*
Parametry:
0   Zobrazi aktualni adresar a jeho cestu
1   Zmeni pracovni adresar dle zadane cesty

Prepinace
/D  Zmeni aktualni jednotku (to se nas asi netyka)

*/
void Cd::run(void) {

	THandle main = search_runing_thread(SHELL);
	std::string name = "";
	if (_arguments.size() == 0) {
		std::string str;
		printf_current_folder(main, &str);
		save_whole_output(str, true);
	}
	else {

	}
	save_whole_output("Zobrazi aktualni adresar nebo se presune na zadanou cestu", true);
}
