#pragma once
#include "File.h"

#include <string>
#include <iostream>

class Stdout : public File {

public:
	
	virtual size_t write(std::string str, size_t flag);
	//Konstruktor
	Stdout::Stdout(std::string name, FileDescriptor* parent, std::string path) : File(name, parent, path) {

	}
};