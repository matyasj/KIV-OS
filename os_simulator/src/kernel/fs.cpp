#include "fs.h"
#include <iostream>
#include <typeinfo>

#include "File.h"

Folder* rootFolder = new Folder("C",nullptr);

THandle openFile(char * fullFilePath, size_t flags)
{
/*	Folder* slozka = new Folder("slozka1", rootFolder);
	Folder* slozka2 = new Folder("slozka2", rootFolder);
	Folder* slozka3 = new Folder("slozka3", rootFolder);
	Folder* slozka4 = new Folder("slozka4", rootFolder);
	
	rootFolder->addFolder(slozka);
	slozka->addFolder(slozka3);
	slozka->addFolder(slozka2);
	slozka3->addFolder(slozka4);
	File* soubor = new File("soubor1.txt", slozka);
	File* soubor2 = new File("soubor2.txt", slozka);
	slozka4->addFile(soubor);
	slozka4->addFile(soubor2);
	slozka4->printChildren();


	std::cout << "Hledam soubor: " << fullFilePath << "\n";*/
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	if (partsOfPath[0] != "C") {
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
					return nullptr;
				}
				foundFile->setOpened();
				return foundFile;
			}
			else {
				std::cout << "FILE NOT FOUND\n" ;
			}
		}else if(tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}else {
			std::cout << "BAD PATH\n";
		}
	}
	std::cout << "FILE NOT FOUND!\n";
	
	return nullptr;
}

THandle createFile(char * fullFilePath, size_t flags)
{
	std::cout << "Vytvarim soubor: " << fullFilePath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	if (partsOfPath[0] != "C") {
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				std::cout << "FILE ALREADY EXISTS!\n";
				File *foundFile = tmpFolder->getFileByName(partsOfPath[i]);
				tmpFolder->printChildren();
				return foundFile;
			}
			else {
				File* newFile = new File(partsOfPath[i], tmpFolder);
				tmpFolder->addFile(newFile);
				tmpFolder->printChildren();
				return newFile;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			std::cout << "BAD PATH\n";
		}
	}
	return (THandle)nullptr;
}

int writeFile(THandle file, char * buffer)
{
	File *tmpFile = (File *)file;
	int numberOfBytes = (int)tmpFile->write(buffer);
	if (numberOfBytes == -1) {
		std::cout << "File " << tmpFile->name << " is NOT opened\nUnable to write into the file\n";
	}
	return numberOfBytes;
}

char * readFile(THandle file)
{
	File *tmpFile = (File *)file;
	std::string str = tmpFile->content;
	char* a = new char[str.size()];
	for (int i = 0; i < str.size(); i++) {
		a[i] = str[i];
	}
	return a;
}

bool closeFile(THandle file)
{
	File *tmpFile = (File *)file;
	return tmpFile->setClosed();
}

bool createFolder(char * fullFolderPath)
{
	std::cout << "Vytvarim soubor: " << fullFolderPath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);

	if (partsOfPath[0] != "C") {
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				std::cout << "FOLDER ALREADY EXISTS!\n";
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);
				tmpFolder->printChildren();
				//return foundFolder;
				return false;
			}
			else {
				Folder* newFolder = new Folder(partsOfPath[i], tmpFolder);
				tmpFolder->addFolder(newFolder);
				tmpFolder->printChildren();
				return true;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			std::cout << "Prochazim slozkou " << partsOfPath[i] << "\n";
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			std::cout << "BAD PATH\n";
		}
	}

	return false;
}

bool deleteFolder(char * fullFolderPath)
{
	std::cout << "Mazu soubor: " << fullFolderPath << "\n";
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);

	if (partsOfPath[0] != "C") {
		std::cout << "Bad path of file (root C is missing)!\n";
		return NULL;
	}
	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				bool a = tmpFolder->removeFolder(partsOfPath[i]);;
				return a;
			}
			else {
				std::cout << "FOLDER NOT FOUND";
				return false;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			std::cout << "Prochazim slozkou " << partsOfPath[i] << "\n";
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			std::cout << "BAD PATH\n";
		}
	}

	return false;
}

bool deleteFile(char* fullFilePath){
	std::vector<std::string> partsOfPath = parsePath(fullFilePath);

	if (partsOfPath[0] != "C") {
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
				std::cout << "FILE NOT FOUND\n";
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			std::cout << "BAD PATH\n";
		}
	}
	return false;
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
		recursePrintTree(startNode->folders[i], prefix.append("|-"));
	}
	
}