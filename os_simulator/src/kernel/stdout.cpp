#include "stdout.h"
#include <iostream>


size_t Stdout::write(std::string str, size_t flag)
{
	std::cout << str;
	this->append(str);
	return str.size();
}

bool Stdout::setClosed()
{
	return true;
}
