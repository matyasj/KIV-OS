#include"rtl_error.h"




std::string print_error(int type_error) {
	std::string text;
	switch (type_error)
	{
	case errorFileNotFound:
		text = "Soubor nenalezeno";
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
	default:
		text = "neznama chyba";
		break;
	}
	return text;
}