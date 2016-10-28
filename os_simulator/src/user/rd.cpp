#include "rd.h"


/*
Parametry:
1   Smaze dany adresar (na dane ceste)

Nevim jak to udelame bez prepinacu to nesmaze plny adresar
/S - smaze vcetne podaresaru
/Q - smaze bez potvrzovani

Asi bych to udelal ze rovnou smaze vse - ale muzem se zeptat
*/
std::string Rd::run(void)
{
	std::string name = "";
	if (_arguments.size() != 0) name = _arguments.at(0);
	bool fail = Delete_Folder(name, 0);
	if (fail) {
		return "Adresar smazan";
	}
	else {
		_error = true;
		return "Chyba";
	}
}
