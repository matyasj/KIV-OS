#pragma once
#include <string>
#include "FileDescriptor.h"

class File : public FileDescriptor
{
public:
	std::string content;
	bool isOpened;
	int inFilePosition;

	bool setOpened();
	bool setClosed();

	std::string getContentFromPosition();
	size_t write(std::string str);
	bool setPosition(int newPosition);
	File();
	~File();
};

