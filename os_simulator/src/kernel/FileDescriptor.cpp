#include "FileDescriptor.h"

/*
 * Vrac� true, kdy� je FileDescriptor soubor
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
 * Vrac� true, kdy� je FileDescriptor slo�ka
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
