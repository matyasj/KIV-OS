#include "Folder.h"
#include <iostream>

bool Folder::addChild(FileDescriptor file)
{
	if (this->containFile(file.name)) {
		std::cout << "File with name " << file.name << "  already exists.\n";
		return false;
	}
	else {
		this->children.push_back(file);
		return true;
	}
}

bool Folder::removeChild(std::string name)
{
	for (int i = 0; i != this->children.size(); i++) {
		if (this->children[i].name == name) {
			this->children.erase(this->children.begin() + i);
			return true;
		}
	}
	return false;
}

void Folder::printChildren()
{
	std::cout << this->name + "\n";
	for (int i = 0; i != this->children.size(); i++) {
		std::cout << " - " + this->children[i].name + "\n";
	}
}

bool Folder::containFile(std::string name)
{
	for (int i = 0; i != this->children.size(); i++) {
		if (this->children[i].name == name) {
			return true;
		}
	}
	return false;
}

Folder::Folder()
{
}

Folder::~Folder()
{
}
