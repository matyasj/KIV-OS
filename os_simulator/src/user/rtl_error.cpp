#include"rtl_error.h"



/*vypise chybovou zpravu*/
std::string print_error(size_t type_error) {
	std::string text;
	switch (type_error)
	{
	case errorFileNotFound:
		text = "Soubor nenalezen";
		break;
	case errorFileIsUsed:
		text = "Soubor/slozka uz je otevrena";
		break;
	case errorAlreadyExist:
		text = "Soubor/slozka uz existuje";
		break;
	case errorBadPath:
		text = "Spatna cesta";
		break;
	case errorIO:
		text = "Dalsi nespecifikovana IO chyba";
		break;
	case threadNotFound:
		text = "Vlakno nenalezeno";
		break;
	case wrongArgument:
		text = "Spatny argument";
		break;
	case onlyOn:
		text = "Prikaz je pouze out";
		break;
	default:
		text = "Nespecifikovana chyba";
		break;
	}
	return text+"\n";
}