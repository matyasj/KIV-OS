#include "FileDescriptorBlock.h"

int instanceCounter = 0;

size_t FileDescriptorBlock::getId()
{
	return this->id;
}

FileDescriptorBlock::FileDescriptorBlock(std::string path, size_t flags, File* filePointer){
	this->id = instanceCounter;
	this->path = path;
	this->flags = flags;
	this->filePointer = filePointer;
	
	instanceCounter++;

}
