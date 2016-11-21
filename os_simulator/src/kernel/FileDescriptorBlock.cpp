#include "FileDescriptorBlock.h"

/* Statická promìná, která urèuje poèet instancí file - podle této položky, se urèuje */
int instanceCounter = 0;

/*
 * Vrací identifikaèní èíslo položky tabulky file-descriptorù
 */
size_t FileDescriptorBlock::getId()
{
	return this->id;
}

/* Konstruktor */
FileDescriptorBlock::FileDescriptorBlock(std::string path, size_t flags, File* filePointer) {
	this->id = instanceCounter;
	this->path = path;
	this->flags = flags;
	// Pointer na soubor, který FileDescriptor zastupuje
	this->filePointer = filePointer;

	instanceCounter++;
}