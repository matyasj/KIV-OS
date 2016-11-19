#pragma once
#include <vector>
#include <string>
#include "..\common\api.h"
#include "FileDescriptor.h"
#include "Folder.h"


//TODO 
//zpracování absolutní X relativní cesta
// ..
// flags
#define FILE_SEPARATOR '\\'
#define ROOT_FOLDER "C:"

//	Pøíklad cesty: C\slozka1\slozka2\soubor.txt   Nazvy slozek a souboru mohou obsahovat napr. i mezery.
THandle openFile(int procesId, std::string fullFilePath, size_t flags);
THandle createFile(int procesId, std::string fullFilePath, size_t flags);

int writeFile(THandle file, std::string buffer, size_t flag);
bool setInFilePosition(THandle file, int newPosition);
int appendFile(THandle file, std::string buffer);
std::string readFile(THandle file);
bool closeFile(THandle file);
bool deleteFileByPath(std::string fullFilePath);
bool deleteFile(THandle file);

THandle createFolder(std::string fullFolderPath);
bool deleteFolderByPath(std::string fullFolderPath);
bool deleteFolder(THandle folder);

std::vector<std::string> parsePath(std::string path);

void printFSTree();
void recursePrintTree(Folder* startNode, std::string prefix);

bool containRoot(std::string fullFolderPath);
std::string getAbsolutePathFromRelative(int procesId, std::string relativePath);

// File Descriptors
THandle putFileIntoFDTable(File* file, size_t flags);
File* getFileByTHandle(THandle fileDescriptor);
File* removeFileFromFDTable(THandle fileDescriptor);

// Testy prav na cteni/zapis
bool canRead(THandle fileDescriptor);
bool canWrite(THandle fileDescriptor);
bool canOpen(std::string fullFilePath, size_t flags);


// Standartni vstupy/vystupy
THandle getStdOut();
THandle getStdIn();
void init(); // init metoda

// Create Pipe
bool createPipe(THandle* input, THandle *output);
//FileDescriptor getFileByName(char* fileName);