#pragma once
#include "File.h"

#include <string>

/*
 * Instance t�to t��dy reprezentuje standardn� v�stup program� b��c�ch v OS
 * Instance se chovaj� jako soubor
 * D�d� od t��di File
 */
class Stdout : public File {

public:
	/* zapisuje ��dku na standardn� v�stup */
	virtual size_t write(std::string str, size_t flag);
	/* Obsluhuje ud�lost uzav�en� standardn�ho v�stupu */
	virtual bool setClosed();
	//Konstruktor
	Stdout::Stdout(std::string name, std::string path) : File(name, NULL, path) {

	}
};