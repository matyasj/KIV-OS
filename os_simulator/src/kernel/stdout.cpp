#include "stdout.h"
#include <iostream>

/*
 * Zapisuje ��dku na standardn� v�stup
 */
size_t Stdout::write(std::string str, size_t flag)
{
	std::cout << str;
	this->append(str);
	return str.size();
}

/*
 * Obsluhuje ud�lost uzav�en� standardn�ho v�stupu
 */
bool Stdout::setClosed()
{
	// Pouze o�et�uje pokus o uzav�en� standardn�ho vstupu
	return true;
}
