#include "fs.h"
#include <iostream>
#include "File.h"

THandle openFile(char * fileName, size_t flags)
{
	File* newFile = new File();
	return newFile;
}

THandle createFile(char * fileName, size_t flags)
{
	return THandle();
}

int wirteFile(THandle file, char * buffer)
{
	return 0;
}

char * readFile(THandle file)
{
	return nullptr;
}

bool closeFile(THandle file)
{
	return false;
}

bool createFolder(char * folderName)
{
	return false;
}

bool deleteFolder(char * folderName)
{
	std::cout << "TEsTIK2\n" << folderName << "\n";
	return false;
}
