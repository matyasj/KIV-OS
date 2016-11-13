#include "File.h"
#include "../common/api.h"

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
	std::string content(this->content.substr(this->inFilePosition, this->content.size()));
	return content;
}

size_t File::write(std::string str, size_t flag)
{
	if(this->isOpened) {
		if(flag == 0){
			this->content = this->content.insert(this->inFilePosition, str);
		}
		else if (flag == 1) {
			this->content = this->content.replace(this->inFilePosition,str.size(), str);
		}
		else if (flag == 2) {
			this->content = std::string(str);
		}
		
		this->inFilePosition = this->inFilePosition + (int)str.size();
		return str.size();
	}
	else {
		SetLastError(errorFileIsUsed);
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
		SetLastError(errorFileIsUsed);
		return -1;
	}
	
}

bool File::setPosition(int newPosition)
{
	if (this->content.size() > newPosition && this->isOpened) {
		this->inFilePosition = newPosition;
		return true;
	}
	SetLastError(errorFileIsUsed);
	return false;
	
}

std::string File::read()
{
	return this->content;
}

File::File(std::string name, FileDescriptor* parent, std::string path)
{
	this->name = name;
	this->parrentFolder = parent;
	this->content = *(new std::string);
	this->inFilePosition = 0;
	this->isOpened = false;
	this->type = FILE;
	this->path = path;
}


File::~File()
{
}
