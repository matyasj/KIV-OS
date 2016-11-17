#pragma once
#include "File.h"

#include <string>
#include <iostream>

class Stdin : public File {

public:
	virtual std::string read();
	virtual bool setClosed();

	//Konstruktor
	Stdin::Stdin(std::string name, std::string path) : File(name, NULL, path) {
		
	}
};