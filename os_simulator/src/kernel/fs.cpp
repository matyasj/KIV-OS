#include "fs.h"
#include <iostream>
#include <typeinfo>

#include "File.h"


Folder* rootFolder = new Folder(ROOT_FOLDER,nullptr);

THandle openFile(std::string fullFilePath, size_t flags)
{
	std::cout << "Separator: " << FILE_SEPARATOR << "\n";
	std::cout << "Oteviram soubor: " << fullFilePath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	if (partsOfPath[0] != ROOT_FOLDER) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				std::cout << "Soubor nalezen: " << partsOfPath[i] << "\n";
				File *foundFile = tmpFolder->getFileByName(partsOfPath[i]);
				if (foundFile->isOpened) {
					SetLastError(errorFileIsUsed);
					std::cout << "FILE IS OPENED EXCEPTION\n";
					return nullptr;
				}
				foundFile->setOpened();
				return foundFile;
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
	std::cout << "Vytvarim soubor: " << fullFilePath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	if (partsOfPath[0] != ROOT_FOLDER) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				std::cout << "FILE ALREADY EXISTS!\n";
				File *foundFile = tmpFolder->getFileByName(partsOfPath[i]);
				foundFile->setOpened();
				
				return foundFile;
			}
			else {
				File* newFile = new File(partsOfPath[i], tmpFolder, fullFilePath);
				newFile->setOpened();
				tmpFolder->addFile(newFile);
				
				return newFile;
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
	return (THandle)nullptr;
}

int writeFile(THandle file, std::string buffer, size_t flag)
{
	File *tmpFile = (File *)file;
	int numberOfBytes = (int)tmpFile->write(buffer, flag);
	if (numberOfBytes < 0) {
		std::cout << "File " << tmpFile->name << " is NOT opened\nUnable to write into the file\n";
		SetLastError(errorIO);
	}
	return numberOfBytes;
}

bool setInFilePosition(THandle file, int newPosition)
{
	File *tmpFile = (File *)file;
	return tmpFile->setPosition(newPosition);;
}

int appendFile(THandle file, std::string buffer)
{
	File *tmpFile = (File *)file;
	int numberOfBytes = (int)tmpFile->append(buffer);
	if (numberOfBytes < 0) {
		std::cout << "File " << tmpFile->name << " is NOT opened\nUnable to write into the file\n";
		SetLastError(errorIO);
	}
	return numberOfBytes;
}

std::string readFile(THandle file)
{
	File *tmpFile = (File *)file;
	std::string str(tmpFile->content);
	
	return str;
}

bool closeFile(THandle file)
{
	File *tmpFile = (File *)file;
	std::cout << "Zaviram soubor " << tmpFile->name << ".\n";
	return tmpFile->setClosed();
}

THandle createFolder(std::string fullFolderPath)
{
	std::cout << "Vytvarim slozku: " << fullFolderPath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);

	if (partsOfPath[0] != ROOT_FOLDER) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
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
	std::cout << "Mazu soubor: " << fullFolderPath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);

	if (partsOfPath[0] != ROOT_FOLDER) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return false;
	}
	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				bool a = tmpFolder->removeFolder(partsOfPath[i]);
				printFSTree();
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
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	if (partsOfPath[0] != ROOT_FOLDER) {
		SetLastError(errorBadPath);
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				std::cout << "Mazu soubor: " << partsOfPath[i] << "\n";
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
	File *tmpFile = (File *)file;
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