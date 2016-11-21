#include "stdout.h"
#include <iostream>

/*
 * Zapisuje øádku na standardní výstup
 */
size_t Stdout::write(std::string str, size_t flag)
{
	std::cout << str;
	this->append(str);
	return str.size();
}

/*
 * Obsluhuje událost uzavøení standardního výstupu
 */
bool Stdout::setClosed()
{
	// Pouze ošetøuje pokus o uzavøení standardního vstupu
	return true;
}
