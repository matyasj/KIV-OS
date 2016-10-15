#include "FileDescriptor.h"

// Constructor
bool FileDescriptor::isFile()
{
	if (this->type == FILE) {
		return true;
	}
	else {
		return false;
	}
}

// Destructor
bool FileDescriptor::isFolder()
{
	if (this->type == FOLDER) {
		return true;
	}
	else {
		return false;
	}
}

FileDescriptor::FileDescriptor()
{
}


FileDescriptor::~FileDescriptor()
{
}
