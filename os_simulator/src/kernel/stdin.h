#pragma once
#include "File.h"

#include <string>
#include <iostream>

class Stdin : public File {

public:
	virtual std::string read();

	//Konstruktor
	Stdin::Stdin(std::string name, FileDescriptor* parent, std::string path) : File(name, parent, path) {
		
	}
};