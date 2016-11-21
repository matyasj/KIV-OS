#include "stdin.h"
#include <iostream>
#include <string>

/*
 * Vrací øádku ze standardního vstupu
 */
std::string Stdin::read()
{
	std::string line;
	std::cin.clear();
	std::getline(std::cin, line);
	// Kdy je na std in EOF vloí \0 (EOF znak)
	if (std::cin.eof()) {
		line = line + '\0';
	}
	else {
		// Vloí znak konce øádky
		line = line + '\n';
	}
	return line;
}
/*
 * Obsluhuje událost uzavøení standardního
 */
bool Stdin::setClosed()
{
	// Pouze ošetøuje pokus o uzavøení standardního vstupu
	return true;
}
