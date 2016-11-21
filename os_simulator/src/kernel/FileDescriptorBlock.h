#pragma once
#include <string>
#include "File.h"

/*
 * Objekty t��dy FIleDescriptorBlok reprezentuj� polo�ky tabulky FileDescriptor�.
 */
class FileDescriptorBlock
{
public:
	/* Identifika�n� ��slo FileDescriptoru */
	size_t id;
	/* Cel� cesta k souboru, kter� konkr�tn� File-descriptor zastupuje */
	std::string path;
	/* Flags ur�uje pr�va, se kter�mi je soubor otev�en */
	size_t flags;
	/* Ukazatel na soubor, kter� File-descriptor zastupuje */
	File* filePointer;

	/* Vrac� identifika�n� ��slo polo�ky tabulky file-descriptor� */
	size_t getId();
	/* Konstruktor */
	FileDescriptorBlock(std::string path, size_t flags, File* filePointer);
};