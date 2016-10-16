#pragma once

#include "FileDescriptor.h"

#include <vector>
#include <string>
#include "File.h"

class Folder : public FileDescriptor
{
public:
	// vektor instanci podsouboru
	std::vector<File* > files;
	std::vector<Folder* > folders;
	Folder* parrentFolder;
	
	// vektor stringu resp. jmen podsouboru (podslozek)
	std::vector<std::string> getFiles();
	bool addFile(File* file);
	bool addFolder(Folder* file);

	bool removeFile(std::string name);
	bool removeFolder(std::string name);

	bool containFile(std::string name);
	bool containFolder(std::string name);

	File* Folder::getFileByName(std::string name);
	Folder* Folder::getFolderByName(std::string name);

	void printChildren();

	Folder(std::string name, Folder* parent);
	~Folder();
};