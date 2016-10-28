#include "Folder.h"
#include <iostream>

bool Folder::addFile(File* file)
{
	if (this->containFile(file->name)) {
		std::cout << "File with name " << file->name << "  already exists.\n";
		return false;
	}
	else {
		this->files.push_back(file);
		file->parrentFolder = this;

		return true;
	}
}
bool Folder::addFolder(Folder* folder)
{
	if (this->containFolder(folder->name)) {
		std::cout << "File with name " << folder->name << "  already exists.\n";
		return false;
	}
	else {
		this->folders.push_back(folder);
		return true;
	}
}

bool Folder::removeFile(std::string name)
{
	for (int i = 0; i < this->files.size(); i++) {
		if (this->files[i]->name == name) {
			if (this->files[i]->isOpened) {
				std::cout << "FILE IS NOT CLOSED. YOU HAVE TO CLOSE FILE BEFORE DELETE!\n";
				return false;
			}
			else {
				this->files.erase(this->files.begin() + i);
				return true;
			}
		}
	}
	std::cout << "FILE NOT FOUND\n";
	return false;
}

bool Folder::removeFolder(std::string name)
{
	for (int i = 0; i != this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			this->folders.erase(this->folders.begin() + i);
			return true;
		}
	}
	
	return false;
}

void Folder::printChildren()
{
	std::cout << this->name + "\n";
	for (int i = 0; i != this->folders.size(); i++) {
		std::cout << "|- FOLDER::" + this->folders[i]->name + "\n";
	}
	for (int i = 0; i != this->files.size(); i++) {
		std::cout << "|- FILE::" + this->files[i]->name + "\n";
	}
}

bool Folder::containFile(std::string name)
{
	for (int i = 0; i < this->files.size(); i++) {
		if (this->files[i]->name == name) {
			return true;
		}
	}
	return false;
}

bool Folder::containFolder(std::string name)
{
	for (int i = 0; i < this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			return true;
		}
	}
	return false;
}

File* Folder::getFileByName(std::string name)
{
	if (this->containFile(name)) {

		for (int i = 0; i < this->files.size(); i++) {
			if (this->files[i]->name == name) {
				return this->files[i];
			}
		}
	}
	return (File* )nullptr;
	
}

Folder* Folder::getFolderByName(std::string name)
{
	if (this->containFolder(name)) {
		for (int i = 0; i < this->folders.size(); i++) {
			if (this->folders[i]->name == name) {
				return this->folders[i];
				
			}
		}
	}
	return (Folder*)nullptr;

}

Folder::Folder(std::string name, Folder* parent)
{
	this->name = name;
	this->type = FOLDER;
	this->parentFolder = parent;

}

Folder::~Folder()
{
}
