#include "exit.h"

/*
Ukonci konzoli
Bez parametru

*/
std::string Exit::run(void) {
	
	save_whole_output("Console ended.", true);

	return "";
}