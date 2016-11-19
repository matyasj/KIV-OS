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
		folder->parentFolder = this;
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
	bool result = true;
	for (int i = 0; i != this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			//Recursion of removing subfolders
			std::vector<Folder*>::iterator it = this->folders[i]->folders.begin();
			int numberOfOpenedFolders = 0;
			while (it != this->folders[i]->folders.end()) {
				bool success= this->folders[i]->removeFolder((*it)->name);
				if (!success) {
					it++;
					numberOfOpenedFolders++;
					result = (result && success);
				}
				else {
					it = this->folders[i]->folders.begin();
					it += numberOfOpenedFolders;
					continue;
				}
			}
			
			// Mazani souboru
			std::vector<File*>::iterator fileIt = this->folders[i]->files.begin();
			int numberOfOpenedFiles = 0;
			while (fileIt != this->folders[i]->files.end()) {
				std::string name((*fileIt)->name);
				bool success = this->folders[i]->removeFile((*fileIt)->name);
				if (!success) {
					result = (result && success);
					fileIt++;
					numberOfOpenedFiles++;
				}
				else {
					fileIt = this->folders[i]->files.begin();
					fileIt += numberOfOpenedFiles;
					continue;
				}
			}
			if (result) {
				this->folders.erase(this->folders.begin() + i);
				return true;
			}
			else {
				return false;
			}
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
