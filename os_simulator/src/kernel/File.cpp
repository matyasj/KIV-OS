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
	this->content = this->content.insert(this->inFilePosition, str);
	return str.size();
}

bool File::setPosition(int newPosition)
{
	if (this->content.size() <= newPosition) {
		this->inFilePosition;
		return true;
	}
	return false;
	
}

File::File()
{
}


File::~File()
{
}
