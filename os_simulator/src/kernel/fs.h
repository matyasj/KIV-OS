#pragma once
#include <vector>
#include <string>
#include "..\common\api.h"
#include "FileDescriptor.h"
#include "Folder.h"

#define FILE_SEPARATOR '\\'
#define ROOT_FOLDER "C:"

//	Pøíklad cesty: C\slozka1\slozka2\soubor.txt   Nazvy slozek a souboru mohou obsahovat napr. i mezery.
THandle openFile(std::string fullFilePath, size_t flags);
THandle createFile(std::string fullFilePath, size_t flags);

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


// File Descriptors
THandle putFileIntoFDTable(File* file);
File* getFileByTHandle(THandle fileDescriptor);
File* removeFileFromFDTable(THandle fileDescriptor);
void init();
THandle getStdOut();
THandle getStdIn();

// Create Pipe
bool createPipe(THandle* input, THandle *output);
//FileDescriptor getFileByName(char* fileName);