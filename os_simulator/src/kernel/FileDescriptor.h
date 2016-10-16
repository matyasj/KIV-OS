#pragma once

#include <string>

#define FILE 'f'
#define FOLDER 'd'
#define MAX_FILES 255

class FileDescriptor
{
public:
	char type;
	std::string name;

	bool isFile();
	bool isFolder();
	FileDescriptor();
	~FileDescriptor();
};

