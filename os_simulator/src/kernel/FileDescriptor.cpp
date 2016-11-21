#include "FileDescriptor.h"

/*
 * Vrací true, když je FileDescriptor soubor
 */
bool FileDescriptor::isFile()
{
	if (this->type == FILE) {
		return true;
	}
	else {
		return false;
	}
}

/*
 * Vrací true, když je FileDescriptor složka
 */
bool FileDescriptor::isFolder()
{
	if (this->type == FOLDER) {
		return true;
	}
	else {
		return false;
	}
}

/* Konstruktor */
FileDescriptor::FileDescriptor()
{
}

/* Destruktor */
FileDescriptor::~FileDescriptor()
{
}
