#pragma once
#include "File.h"

#include <string>
#include <iostream>

class Stdout : public File {

public:
	
	virtual size_t write(std::string str, size_t flag);
	virtual bool setClosed();
	//Konstruktor
	Stdout::Stdout(std::string name, std::string path) : File(name, NULL, path) {

	}
};