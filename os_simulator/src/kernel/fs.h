#pragma once
#define FILE_SEPARATOR '/'
#include <vector>
#include <string>
#include "..\common\api.h"
#include "FileDescriptor.h"
#include "Folder.h"

//	Pøíklad cesty: C/slozka1/slozka2/soubor.txt
THandle openFile(char* fullFilePath, size_t flags);
THandle createFile(char* fullFilePath, size_t flags);

int writeFile(THandle file, char* buffer);
char* readFile(THandle file);
bool closeFile(THandle file);
bool deleteFile(char* fullFilePath);

bool createFolder(char* fullFolderPath);
bool deleteFolder(char* fullFolderPath);

std::vector<std::string> parsePath(std::string path);

void printFSTree();
void recursePrintTree(Folder* startNode, std::string prefix);


//FileDescriptor getFileByName(char* fileName);