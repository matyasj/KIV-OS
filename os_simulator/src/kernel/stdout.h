#pragma once
#include "File.h"

#include <string>

/*
 * Instance této tøídy reprezentuje standardní výstup programù bìžících v OS
 * Instance se chovají jako soubor
 * Dìdí od tøídi File
 */
class Stdout : public File {

public:
	/* zapisuje øádku na standardní výstup */
	virtual size_t write(std::string str, size_t flag);
	/* Obsluhuje událost uzavøení standardního výstupu */
	virtual bool setClosed();
	//Konstruktor
	Stdout::Stdout(std::string name, std::string path) : File(name, NULL, path) {

	}
};