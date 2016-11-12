#pragma once
#include <string>
#include "File.h"


class FileDescriptorBlock
{
public:
	size_t id;
	std::string path;
	size_t flags;
	File* filePointer;
	size_t getId();
	FileDescriptorBlock(std::string path, size_t flags, File* filePointer);
	//~FileDescriptorBlock();

};