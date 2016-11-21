#include "FileDescriptorBlock.h"

/* Statick� prom�n�, kter� ur�uje po�et instanc� file - podle t�to polo�ky, se ur�uje */
int instanceCounter = 0;

/*
 * Vrac� identifika�n� ��slo polo�ky tabulky file-descriptor�
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
	// Pointer na soubor, kter� FileDescriptor zastupuje
	this->filePointer = filePointer;

	instanceCounter++;
}