#include "fs.h"
#include <iostream>
#include <typeinfo>

#include "File.h"
#include "FileDescriptorBlock.h"
#include "stdin.h"
#include "stdout.h"


Folder* rootFolder = new Folder(ROOT_FOLDER,nullptr);
std::vector<FileDescriptorBlock*> FileDescriptorTable;

THandle openFile(std::string fullFilePath, size_t flags)
{
	if (!containRoot(fullFilePath)) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				std::cout << "Soubor nalezen: " << partsOfPath[i] << "\n";
				File *foundedFile = tmpFolder->getFileByName(partsOfPath[i]);
				if (foundedFile->isOpened) {
					SetLastError(errorFileIsUsed);
					std::cout << "FILE IS OPENED EXCEPTION\n";
					return nullptr;
				}
				foundedFile->setOpened();
				THandle newFileDescriptor = putFileIntoFDTable(foundedFile);
				return newFileDescriptor;
			}
			else {
				SetLastError(errorFileNotFound);
				std::cout << "FILE NOT FOUND\n" ;
			}
		}else if(tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}else {
			SetLastError(errorBadPath);
			std::cout << "BAD PATH\n";
		}
	}
	SetLastError(errorFileNotFound);
	std::cout << "FILE NOT FOUND!\n";
	
	return nullptr;
}

THandle createFile(std::string fullFilePath, size_t flags)
{
	if (fullFilePath == "CONOUT$") {
		return getStdOut();
	}
	else if (fullFilePath == "CONIN$") {
		return getStdIn();
	}
	if (!containRoot(fullFilePath)) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}

	std::cout << "Vytvarim soubor: " << fullFilePath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				std::cout << "FILE ALREADY EXISTS!\n";
				File *foundFile = tmpFolder->getFileByName(partsOfPath[i]);
				foundFile->setOpened();
				THandle newFileDescriptor = putFileIntoFDTable(foundFile);
				if ((size_t)newFileDescriptor != -1) {
					return newFileDescriptor;
				}
				else {
					std::cout << "Chyba neni mozne takto otevrit soubor \n";
					return newFileDescriptor;
				}
				
			}
			else {
				File* newFile = new File(partsOfPath[i], tmpFolder, fullFilePath);
				newFile->setOpened();
				tmpFolder->addFile(newFile);
				THandle newFileDescriptor = putFileIntoFDTable(newFile);
				
				return newFileDescriptor;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			std::cout << "BAD PATH\n";
		} 
	}
	return (THandle)-1;
}

int writeFile(THandle file, std::string buffer, size_t flag)
{
	File *tmpFile = getFileByTHandle(file);
	Stdout* out = dynamic_cast<Stdout*>(tmpFile);
	int numberOfBytes = (int)tmpFile->write(buffer, flag);
	if (numberOfBytes < 0) {
		std::cout << "File " << tmpFile->name << " is NOT opened\nUnable to write into the file\n";
		SetLastError(errorIO);
	}
	return numberOfBytes;
}

bool setInFilePosition(THandle file, int newPosition)
{
	File *tmpFile = getFileByTHandle(file);
	return tmpFile->setPosition(newPosition);;
}

int appendFile(THandle file, std::string buffer)
{
	File *tmpFile = getFileByTHandle(file);
	int numberOfBytes = (int)tmpFile->append(buffer);
	if (numberOfBytes < 0) {
		std::cout << "File " << tmpFile->name << " is NOT opened\nUnable to write into the file\n";
		SetLastError(errorIO);
	}
	return numberOfBytes;
}

std::string readFile(THandle file)
{
	File *tmpFile = getFileByTHandle(file);
	std::string str(tmpFile->read());
	
	return str;
}

bool closeFile(THandle file)
{
	File *tmpFile = getFileByTHandle(file);
	std::cout << "Zaviram soubor " << tmpFile->name << ".\n";
	return tmpFile->setClosed();
}

THandle createFolder(std::string fullFolderPath)
{
	if (!containRoot(fullFolderPath)) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	std::cout << "Vytvarim slozku: " << fullFolderPath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);

	
	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {

		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				std::cout << "FOLDER ALREADY EXISTS!\n";
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);
				//tmpFolder->printChildren();
				
				return false;
			}
			else {
				Folder* newFolder = new Folder(partsOfPath[i], tmpFolder);
				tmpFolder->addFolder(newFolder);
				//tmpFolder->printChildren();
				return newFolder;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			std::cout << "BAD PATH\n";
			return false;
		}
	}

	return false;
}

bool deleteFolderByPath(std::string fullFolderPath)
{
	if (containRoot(fullFolderPath)) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return false;
	}
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);

	
	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				bool a = tmpFolder->removeFolder(partsOfPath[i]);
				return a;
			}
			else {
				SetLastError(errorFileNotFound);
				std::cout << "FOLDER NOT FOUND";
				return false;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			std::cout << "BAD PATH\n";
			return false;
		}
	}
	SetLastError(errorFileNotFound);

	return false;
}

bool deleteFolder(THandle folder)
{
	Folder *tmpFolder = (Folder *)folder;
	std::string tmpPath = "";
	tmpPath.insert(0, tmpFolder->name);
	while (tmpFolder->parentFolder != nullptr) {
		tmpFolder = tmpFolder->parentFolder;
		tmpPath.insert(0,tmpFolder->name + FILE_SEPARATOR);
	}
	
	deleteFolderByPath(tmpPath);
	
	return false;
}

bool deleteFileByPath(std::string fullFilePath){
	if (containRoot(fullFilePath)) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				return tmpFolder->removeFile(partsOfPath[i]);
			}
			else {
				SetLastError(errorFileNotFound);
				std::cout << "FILE NOT FOUND\n";
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			std::cout << "BAD PATH\n";
			return false;
		}
	}
	SetLastError(errorFileNotFound);
	return false;
}

bool deleteFile(THandle file)
{
	File *tmpFile = getFileByTHandle(file);
	std::string path = tmpFile->path;
	return deleteFileByPath(path);
}

std::vector<std::string> parsePath(std::string path) {
	std::vector<std::string> arrayOfPartsOfPath;
	std::string tmp = "";
	for (int i = 0; i < path.length(); i++) {
		if (path[i] == FILE_SEPARATOR) {
			arrayOfPartsOfPath.push_back(tmp);
			tmp = "";
		}
		else {
			tmp += path[i];
		}
			
	}
	arrayOfPartsOfPath.push_back(tmp);
	
	return arrayOfPartsOfPath;
}

void printFSTree()
{
	recursePrintTree(rootFolder, "");
}

void recursePrintTree(Folder * startNode, std::string prefix)
{
	std::cout << "" << prefix << startNode->name << "\n";

	for (int i = 0; i < startNode->files.size(); i++) {
		std::cout << prefix << "|-" << startNode->files[i]->name << "\n";
	}

	for (int i = 0; i < startNode->folders.size(); i++) {
		recursePrintTree(startNode->folders[i], prefix + "|-");
	}
	
}

bool containRoot(std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);

	if (partsOfPath[0] != ROOT_FOLDER) {
		//SetLastError(errorBadPath);
		return false;
	}
	return true;
}

/*
 * @return FileDescriptor of new created FileDescriptor
*/
THandle putFileIntoFDTable(File* file) {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	FileDescriptorBlock* newFD = new FileDescriptorBlock(file->path, FILE_SHARE_READ, file);
	FileDescriptorTable.push_back(newFD);

	return (THandle)newFD->getId();
}

File* getFileByTHandle(THandle fileDescriptor) {
	size_t id = (size_t)fileDescriptor;
	for (size_t i = 0; i < FileDescriptorTable.size(); i++)
	{
		if (FileDescriptorTable[i]->getId() == id) {
			return FileDescriptorTable[i]->filePointer;
		}
	}
	return nullptr;
}

void init()
{
	std::string path = ROOT_FOLDER;
	path += "name";
	Stdin* std_in = new Stdin("std_in", NULL, path);
	Stdout* std_out = new Stdout("std_out", NULL, path);

	FileDescriptorBlock* std_inFD = new FileDescriptorBlock(std_in->path, FILE_SHARE_READ, std_in);
	FileDescriptorBlock* std_outFD = new FileDescriptorBlock(std_in->path, FILE_SHARE_READ, std_out);

	FileDescriptorTable.push_back(std_inFD);
	FileDescriptorTable.push_back(std_outFD);
	std::cout << "Cykl: \n";
	for (int i = 0; i < FileDescriptorTable.size(); i++) {
		std::cout << FileDescriptorTable[i]->filePointer->name << "\n";
	}
	
}

THandle getStdIn() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	THandle std_in = (THandle)0;

	return std_in;
}

THandle getStdOut() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	THandle std_out = (THandle)1;

	return std_out;
}