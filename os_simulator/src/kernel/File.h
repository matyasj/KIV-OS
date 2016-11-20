#pragma once

#include <string>
#include "FileDescriptor.h"
#include <mutex>

class File : public FileDescriptor
{
public:
	std::string content;
	bool isOpened;
	int inFilePosition;
	FileDescriptor* parrentFolder;
	std::string path;
	std::mutex mtx;

	bool setOpened();
	virtual bool setClosed();

	std::string getContentFromPosition();
	virtual size_t write(std::string str, size_t flag);
	size_t append(std::string str);
	bool setPosition(int newPosition);
	virtual std::string read();
	
	//Konstruktor
	File(std::string name, FileDescriptor* parent, std::string path);
	//Destruktor
	~File();
};

