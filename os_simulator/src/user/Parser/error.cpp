#include<string>
#include<iostream>
#include"error.h"

bool has_error = false;
int parser_error(int error) {
	std::string str;
	switch (error) {
	case UNKNOWN_COMMAND:
		str = "Neznamy prikaz";
		break;
	case COUNT_OF_ARGUMENT:
		str = "Nespravny pocet argumentu";
		break;
	case MORE_WORDS_ARGUMENT:
		str = "Jedna se o viceslovny argument. Vlozte ho do uvozovek";
		break;
	case MORE_MARKS:
		str = "Argument obsahuje vice jak dve uvozovky.";
		break;
	case ONE_MARKS:
		str = "Argument obsahuje pouze jednu uvozovku.";
		break;
	case REDIRECT_ERROR:
		str = "Chyba pri presmerovani souboru";
		break;
	case TOO_MANY_REDIRECT:
		str = "Zadal jste k jednomu prikazu prilis presmerovani. Je dovoleno jen jedno";
		break;
	default:
		str = "Nespecifikovana chyba";
	}
	has_error = true;
	std::cout << "Chyba: " << str << std::endl;
	return ERROR;
}
bool parser_has_error(){
	return has_error;
}