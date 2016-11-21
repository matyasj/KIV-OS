#pragma once
#include "File.h"

#include <string>

/*
 * Instance této tøídy reprezentuje standardní vstup programù bìžících v OS
 * Instance se chovají jako soubor
 * Dìdí od tøídi File
 */
class Stdin : public File {

public:
	/* Vrací øádku ze standardního vstupu */
	virtual std::string read();
	/* Obsluhuje událost uzavøení standardního vstupu */
	virtual bool setClosed();

	//Konstruktor
	Stdin::Stdin(std::string name, std::string path) : File(name, NULL, path) {

	}
};