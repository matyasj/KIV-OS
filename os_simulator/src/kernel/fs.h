#pragma once
#include <vector>
#include <string>
#include "..\common\api.h"
#include "FileDescriptor.h"
#include "Folder.h"


#define FILE_SEPARATOR '\\'
#define ROOT_FOLDER "C:"
#define UPPER_DIRECTORY ".."

//	P��klad cesty: C\slozka1\slozka2\soubor.txt   Nazvy slozek a souboru mohou obsahovat napr. i mezery.
THandle openFile(int procesId, std::string fullFilePath, size_t flags);
THandle createFile(int procesId, std::string fullFilePath, size_t flags);

int writeFile(THandle file, std::string buffer, size_t flag);
bool setInFilePosition(THandle file, int newPosition);
int appendFile(THandle file, std::string buffer);
std::string readFile(THandle file);
bool closeFile(THandle file);
bool deleteFileByPath(int procesId, std::string fullFilePath);
bool deleteFile(int procesId, THandle file);

THandle createFolder(int procesId, std::string fullFolderPath);
bool deleteFolderByPath(int procesId, std::string fullFolderPath);
bool deleteFolder(int procesId, THandle folder);

std::vector<std::string> parsePath(std::string path);
std::vector<std::string> correctPath(std::vector<std::string> pathParts);
std::vector<std::string> checkPath(int proces_id, std::string fullPath);

void printFSTree();
void recursePrintTree(Folder* startNode, std::string prefix);
std::string printFolder(int procesId, std::string fullFolderPath);
bool changeWorkDirectory(int procesId, std::string fullFolderPath);

bool containRoot(std::string fullFolderPath);
bool containColon(std::string str);
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