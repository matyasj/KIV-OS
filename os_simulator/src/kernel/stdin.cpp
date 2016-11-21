#include "stdin.h"
#include <iostream>
#include <string>

/*
 * Vrac� ��dku ze standardn�ho vstupu
 */
std::string Stdin::read()
{
	std::string line;
	std::cin.clear();
	std::getline(std::cin, line);
	// Kdy� je na std in EOF vlo�� \0 (EOF znak)
	if (std::cin.eof()) {
		line = line + '\0';
	}
	else {
		// Vlo�� znak konce ��dky
		line = line + '\n';
	}
	return line;
}
/*
 * Obsluhuje ud�lost uzav�en� standardn�ho
 */
bool Stdin::setClosed()
{
	// Pouze o�et�uje pokus o uzav�en� standardn�ho vstupu
	return true;
}
