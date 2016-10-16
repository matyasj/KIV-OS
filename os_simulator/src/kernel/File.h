#pragma once

#include <string>
#include "FileDescriptor.h"

class File : public FileDescriptor
{
public:
	std::string content;
	bool isOpened;
	int inFilePosition;
	FileDescriptor* parrentFolder;

	bool setOpened();
	bool setClosed();

	std::string getContentFromPosition();
	size_t write(std::string str);
	size_t append(std::string str);
	bool setPosition(int newPosition);
	//Konstruktor
	File(std::string name, FileDescriptor* parent);
	//Destruktor
	~File();
};

