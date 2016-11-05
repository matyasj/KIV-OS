#include "echo.h"


/*
Parametry:
0	Vypise jestli je ECHO rezim zapnut (nevim jestli budem delat)
1	ON / OFF - Vypne/Zapne zobrazovani zprav (ECHO rezim)
1   Vypise zadanou zpravu

*/
void Echo::run(void)
{
	std::string arg = "";
	if(_arguments.size()!=0) arg = _arguments.at(0);

	save_whole_output(arg, true);
}
