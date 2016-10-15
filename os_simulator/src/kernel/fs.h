#pragma once

#include "..\common\api.h"
#include "FileDescriptor.h"
#include <vector>


std::vector<FileDescriptor> data;

THandle openFile(char* fileName, size_t flags);
THandle createFile(char* fileName, size_t flags);

int wirteFile(THandle file, char* buffer);
char* readFile(THandle file);
bool closeFile(THandle file);

bool createFolder(char* folderName);
bool deleteFolder(char* folderName);

FileDescriptor getFileByName(char* fileName);