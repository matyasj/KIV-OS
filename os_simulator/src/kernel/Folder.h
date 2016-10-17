#pragma once

#include "FileDescriptor.h"

#include <vector>
#include <string>
#include "File.h"

/*Trida, co reprezentuje Soubor*/
class Folder : public FileDescriptor
{
public:
	// vektor incstanci souboru
	std::vector<File* > files;
	// vektor incstanci slozek
	std::vector<Folder* > folders;
	// Rodicovska slozka
	Folder* parrentFolder;
	
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