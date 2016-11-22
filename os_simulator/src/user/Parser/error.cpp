#include<string>
#include<iostream>
#include"error.h"

Error::Error()
{
}

Error::~Error()
{
}
//ulozi chybu parseru
int Error::parser_error(int error) {
	if (!this->has_error) {
		this->last_error = error;
		this->has_error = true;
	}
	return ERROR_INSTRUCTION;
}
//vytiskne ulozenou chybovou hlasku
std::string Error::print_last_error() {
	std::string str;
	switch (this->last_error) {
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
		str = "Zadal jste k jednomu prikazu prilis presmerovani. Je dovoleno jen jedno vstupni a jedno vystupni";
		break;
	case EXIT_IN_PIPE:
		str = "EXIT nemuze byt v pipe ani byt presmerovan";
		break;
	case SHELL_IN_PIPE:
		str = "SHELL nemuze byt v pipe ani byt presmerovan";
		break;
	default:
		str = "Nespecifikovana chyba";
	}
	return str+"\n";
}
//vyresetuje aktualni nastaveni
void Error::reset_parser_error()
{
	this->has_error = false;
}
// ma parsesr chybu?
bool Error::parser_has_error()
{
	return this->has_error;
}
