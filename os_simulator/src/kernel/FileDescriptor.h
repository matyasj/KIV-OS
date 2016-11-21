#pragma once

#include <string>

#define FILE 'f'
#define FOLDER 'd'
#define MAX_FILES 255

/*
 * Objekty t��dy FileDescriptor reprezentuj� jednotliv� prvky Filesystemu.
 */
class FileDescriptor
{
public:
	/* Typ - zda je objekt soubor, nebo slo�ka */
	char type;
	/* Jm�no FileDescriptoru (souboru/slo�ky) */
	std::string name;

	/* Vrac� true, kdy� je FileDescriptor soubor */
	bool isFile();
	/* Vrac� true, kdy� je FileDescriptor slo�ka */
	bool isFolder();

	/* Konstruktor */
	FileDescriptor();
	/* Destruktor */
	~FileDescriptor();
};

