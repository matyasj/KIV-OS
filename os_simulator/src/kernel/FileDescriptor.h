#pragma once

#include <string>

#define FILE 'f'
#define FOLDER 'd'

class FileDescriptor
{
public:
	int id;
	char type;
	std::string name;

	bool isFile();
	bool isFolder();
	FileDescriptor();
	~FileDescriptor();
};

