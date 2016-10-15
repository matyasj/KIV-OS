#pragma once

#include <string>

#define FILE 'f'
#define FOLDER 'd'
#define MAX_FILES 255

class FileDescriptor
{
public:
	int id;
	char type;
	std::string name;
	int parrentFolder;

	bool isFile();
	bool isFolder();
	FileDescriptor();
	~FileDescriptor();
};

