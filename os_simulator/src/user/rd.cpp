#include "rd.h"


/*
Parametry:
1   Smaze dany adresar (na dane ceste)

Nevim jak to udelame bez prepinacu to nesmaze plny adresar
/S - smaze vcetne podaresaru
/Q - smaze bez potvrzovani

Asi bych to udelal ze rovnou smaze vse - ale muzem se zeptat
*/
void Rd::run(void) {

	std::string output = "";
	std::string name = "";

	if (_arguments.size() != 0) name = _arguments.at(0);
	bool fail = Delete_Folder(name, 0);
	if (fail) {
		output = "Adresar smazan";
	}
	else {
		_error = true;
		output = "Chyba";
	}

	save_whole_output(output, true);
}
