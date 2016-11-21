#pragma once
#include "File.h"

#include <string>

/*
 * Instance t�to t��dy reprezentuje standardn� vstup program� b��c�ch v OS
 * Instance se chovaj� jako soubor
 * D�d� od t��di File
 */
class Stdin : public File {

public:
	/* Vrac� ��dku ze standardn�ho vstupu */
	virtual std::string read();
	/* Obsluhuje ud�lost uzav�en� standardn�ho vstupu */
	virtual bool setClosed();

	//Konstruktor
	Stdin::Stdin(std::string name, std::string path) : File(name, NULL, path) {

	}
};