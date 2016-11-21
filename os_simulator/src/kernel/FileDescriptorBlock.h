#pragma once
#include <string>
#include "File.h"

/*
 * Objekty tøídy FIleDescriptorBlok reprezentují položky tabulky FileDescriptorù.
 */
class FileDescriptorBlock
{
public:
	/* Identifikaèní èíslo FileDescriptoru */
	size_t id;
	/* Celá cesta k souboru, které konkrétní File-descriptor zastupuje */
	std::string path;
	/* Flags urèuje práva, se kterými je soubor otevøen */
	size_t flags;
	/* Ukazatel na soubor, který File-descriptor zastupuje */
	File* filePointer;

	/* Vrací identifikaèní èíslo položky tabulky file-descriptorù */
	size_t getId();
	/* Konstruktor */
	FileDescriptorBlock(std::string path, size_t flags, File* filePointer);
};