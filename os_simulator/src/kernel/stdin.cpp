#include "stdin.h"
#include <iostream>
#include <string>

std::string Stdin::read()
{
	std::string line;
	std::cin.clear();
	std::getline(std::cin, line);
	if (std::cin.eof()) {
		line = line + '\0';
	}
	else {
		line = line + '\n';
	}
	return line;
}

bool Stdin::setClosed()
{
	return true;
}
