#pragma once
#include "FileDescriptor.h"
#include <vector>
#include <string>

class Folder : public FileDescriptor
{
public:
	// vektor instanci podsouboru
	std::vector<FileDescriptor> children;
	
	// vektor stringu resp. jmen podsouboru (podslozek)
	std::vector<std::string> getFiles();
	bool addChild(FileDescriptor file);
	bool removeChild(std::string name);
	void printChildren();
	bool containFile(std::string name);

	Folder();
	~Folder();
};