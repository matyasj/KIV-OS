#pragma once

#include <string>

#define FILE 'f'
#define FOLDER 'd'
#define MAX_FILES 255

/*
 * Objekty tøídy FileDescriptor reprezentují jednotlivé prvky Filesystemu.
 */
class FileDescriptor
{
public:
	/* Typ - zda je objekt soubor, nebo složka */
	char type;
	/* Jméno FileDescriptoru (souboru/složky) */
	std::string name;

	/* Vrací true, když je FileDescriptor soubor */
	bool isFile();
	/* Vrací true, když je FileDescriptor složka */
	bool isFolder();

	/* Konstruktor */
	FileDescriptor();
	/* Destruktor */
	~FileDescriptor();
};

