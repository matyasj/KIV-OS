#include "File.h"

bool File::setOpened()
{
	if (this->isOpened) {
		return false;
	}
	else {
		this->isOpened = true;
		this->inFilePosition = 0;
		return true;
	}
}

bool File::setClosed()
{
	this->isOpened = false;
	this->inFilePosition = -1;
	return true;
}

std::string File::getContentFromPosition()
{
	return this->content.substr(this->inFilePosition, this->content.size());
}

size_t File::write(std::string str)
{
	if (this->isOpened) {
		this->content = this->content.insert(this->inFilePosition, str);
		return str.size();
	}
	else {
		return -1;
	}
	
	
}

size_t File::append(std::string str)
{
	if (this->isOpened) {
		this->content.append(str);
		return str.size();
	}
	else {
		return -1;
	}
	
}

bool File::setPosition(int newPosition)
{
	if (this->content.size() <= newPosition) {
		this->inFilePosition;
		return true;
	}
	return false;
	
}

File::File(std::string name, FileDescriptor* parent)
{
	this->name = name;
	this->parrentFolder = parent;
	this->content = "";
	this->inFilePosition = 0;
	this->isOpened = false;
	this->type = FILE;
}


File::~File()
{
}
