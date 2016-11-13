#include "stdin.h"
#include <iostream>
#include <string>

std::string Stdin::read()
{
	std::string line;
	std::cin.clear();
	std::getline(std::cin, line);
	
	return line;
}