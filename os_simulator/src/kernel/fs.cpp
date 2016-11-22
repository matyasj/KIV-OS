#include "fs.h"
#include <iostream>
#include <typeinfo>

#include "File.h"
#include "FileDescriptorBlock.h"
#include "stdin.h"
#include "stdout.h"
#include "pipe.h"
#include "TCB.h"


Folder* rootFolder = new Folder(ROOT_FOLDER, nullptr);
std::vector<FileDescriptorBlock*> FileDescriptorTable;

THandle openFile(int procesId, std::string fullFilePath, size_t flags)
{
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return nullptr;

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {

		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				std::cout << "Soubor nalezen: " << partsOfPath[i] << "\n";
				File *foundedFile = tmpFolder->getFileByName(partsOfPath[i]);
				if (!canOpen(fullFilePath, flags)) {
					SetLastError(errorFileIsUsed);
					std::cout << "FILE IS OPENED EXCEPTION3 (PERMISSION FAILED)\n";
					return nullptr;
				}
				foundedFile->setOpened();
				THandle newFileDescriptor = putFileIntoFDTable(foundedFile, flags);
				return newFileDescriptor;
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
		}
	}
	SetLastError(errorFileNotFound);
	std::cout << "FILE NOT FOUND!\n";

	return nullptr;
}

THandle createFile(int procesId, std::string fullFilePath, size_t flags)
{
	if (fullFilePath == "CONOUT$") {
		return getStdOut();
	}
	else if (fullFilePath == "CONIN$") {
		return getStdIn();
	}

	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return nullptr;

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {

		if (i == (partsOfPath.size() - 1)) {
			if (containColon(partsOfPath[i])) {
				SetLastError(errorBadPath);
				std::cout << "FILE " << partsOfPath[i] << " CONTAIN ':' character!\n";
				return nullptr;
			}

			if (tmpFolder->containFile(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				std::cout << "FILE ALREADY EXISTS!\n";
				File *foundFile = tmpFolder->getFileByName(partsOfPath[i]);
				if (!canOpen(fullFilePath, flags)) {
					SetLastError(errorFileIsUsed);
					std::cout << "FILE IS OPENED EXCEPTION2 (PERMISSION FAILED)\n";
					return nullptr;
				}
				foundFile->setOpened();
				THandle newFileDescriptor = putFileIntoFDTable(foundFile, flags);
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
				THandle newFileDescriptor = putFileIntoFDTable(newFile, flags);

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
	return nullptr;
}

int writeFile(THandle file, std::string buffer, size_t flag)
{
	File *tmpFile = getFileByTHandle(file);
	if (canWrite(file)) {
		int numberOfBytes = (int)tmpFile->write(buffer, flag);
		if (numberOfBytes < 0) {
			std::cout << "FILE " << tmpFile->name << " IS NOT OPENED - UNABLE TO WRITE\n";
			SetLastError(errorIO);
		}
		return numberOfBytes;
	}
	std::cout << "FILE " << tmpFile->name << " HAS NOT WRITE PERMESSION FOR THIS PROCESS - UNABLE TO WRITE\n";
	SetLastError(errorIO);
	return -1;

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
		std::cout << "FILE " << tmpFile->name << " IS NOT OPENED - UNABLE TO WRITE\n";
		SetLastError(errorIO);
	}
	return numberOfBytes;
}

std::string readFile(THandle file)
{
	File *tmpFile = getFileByTHandle(file);
	if (canRead(file)) {
		std::string str(tmpFile->read());
		return str;
	}
	std::cout << "FILE " << tmpFile->name << " HAS NOT READ PERMESSION FOR THIS PROCESS1 - UNABLE TO WRITE\n";
	SetLastError(errorIO);
	return nullptr;
}

bool closeFile(THandle file)
{
	File *tmpFile = removeFileFromFDTable(file);
	if (tmpFile == NULL) {
		return false;
	}
	return tmpFile->setClosed();
}

THandle createFolder(int procesId, std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return nullptr;

	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {

		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				std::cout << "FOLDER ALREADY EXISTS!\n";
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);
				//tmpFolder->printChildren();

				return foundFolder;
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
			return nullptr;
		}
	}

	return nullptr;
}

bool deleteFolderByPath(int procesId, std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

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

/*
	Nikde se nejspis nepouziva mozna smazat
*/
bool deleteFolder(int procesId, THandle folder)
{
	Folder *tmpFolder = (Folder *)folder;
	std::string tmpPath = "";
	tmpPath.insert(0, tmpFolder->name);
	while (tmpFolder->parentFolder != nullptr) {
		tmpFolder = tmpFolder->parentFolder;
		tmpPath.insert(0, tmpFolder->name + FILE_SEPARATOR);
	}

	deleteFolderByPath(procesId, tmpPath);

	return false;
}

bool lockFolder(std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	if (partsOfPath.size() == 0 || !containRoot(fullFolderPath))
		return false;

	Folder *tmpFolder = rootFolder;

	for (int i = 0; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->name == partsOfPath[i]) {
				return tmpFolder->lockFolder();
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i + 1])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i + 1]);
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

bool unLockFolder(std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	if (partsOfPath.size() == 0 || !containRoot(fullFolderPath))
		return false;

	Folder *tmpFolder = rootFolder;

	for (int i = 0; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->name == partsOfPath[i]) {
				return tmpFolder->unLockFolder();
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i + 1])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i + 1]);
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

bool deleteFileByPath(int procesId, std::string fullFilePath) {
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return false;

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

bool deleteFile(int procesId, THandle file)
{
	File *tmpFile = getFileByTHandle(file);
	std::string path = tmpFile->path;
	return deleteFileByPath(procesId, path);
}

std::vector<std::string> parsePath(std::string path) {
	std::vector<std::string> arrayOfPartsOfPath;
	std::string tmp = "";

	for (char charAtPosition : path) {
		if (charAtPosition == FILE_SEPARATOR) {
			arrayOfPartsOfPath.push_back(tmp);
			tmp = "";
		}
		else {
			tmp += charAtPosition;
		}

	}
	if (tmp != "") {
		arrayOfPartsOfPath.push_back(tmp);
	}
	

	return arrayOfPartsOfPath;
}

std::vector<std::string> correctPath(std::vector<std::string> pathParts)
{
	if (pathParts.size() > 1) {
		if (pathParts[1] == UPPER_DIRECTORY) {
			SetLastError(errorBadPath);
			return std::vector<std::string>();
		}
	}
	// Uprava cesty pro nadøazenou složku '..'
	for (int i = 1; i < (pathParts.size() - 1); i++) {
		if (pathParts[i + 1] == UPPER_DIRECTORY) {
			pathParts.erase(pathParts.begin() + (i + 1));
			pathParts.erase(pathParts.begin() + i);
			i -= 2;
		}
	}
	// Uprava cesty pro souèasnou složku '.'
	for (int i = 0; i < (pathParts.size() - 1); i++) {
		if (pathParts[i + 1] == CURRENT_DIRECTORY) {
			pathParts.erase(pathParts.begin() + (i + 1));
		}
	}
	return pathParts;
}

std::vector<std::string> checkPath(int proces_id, std::string fullPath)
{
	std::vector<std::string> partsOfPath;
	if (!containRoot(fullPath)) {
		fullPath = getAbsolutePathFromRelative(proces_id, fullPath);
		partsOfPath = parsePath(fullPath);
		partsOfPath = correctPath(partsOfPath);
		if (partsOfPath.size() == 0) {
			SetLastError(errorBadPath);
			std::cout << "Bad path of File " << fullPath << "\n";
			return std::vector<std::string>();
		}
		if (partsOfPath[0] != ROOT_FOLDER) {
			SetLastError(errorBadPath);
			std::cout << "Bad path of folder " << fullPath << "\n";
			return std::vector<std::string>();
		}
	}
	else {
		partsOfPath = parsePath(fullPath);
		partsOfPath = correctPath(partsOfPath);
	}
	return partsOfPath;
}

void printFSTree()
{
	recursePrintTree(rootFolder, "");
}

void recursePrintTree(Folder * startNode, std::string prefix)
{
	std::cout << "" << prefix << startNode->name << "\n";
	for (File* tmpFile : startNode->files) {
		std::cout << prefix << "|-" << tmpFile->name << "\n";
	}

	for (Folder* tmpFolder : startNode->folders) {
		recursePrintTree(tmpFolder, prefix + "|-");
	}

}

std::string printFolder(int procesId, std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;


	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);

				return foundFolder->printChildren();
			}
			else {
				return tmpFolder->printChildren();
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			std::cout << "BAD PATH\n";
			return std::string();
		}
	}
	SetLastError(errorBadPath);
	std::cout << "BAD PATH\n";
	return std::string();
}

bool changeWorkDirectory(int procesId, std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;

	if (partsOfPath.size() == 1) {
		int id_shell = get_active_thread_by_type(SHELL);
		std::string str = ROOT_FOLDER;
		str += FILE_SEPARATOR;
		int return_value = change_thread_current_folder(id_shell, &(std::string(str)));
		return !return_value;
	}

	for (int i = 1; i < partsOfPath.size(); i++) {
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);
				std::string absolutePath = "";
				for (std::string name : partsOfPath) {
					absolutePath += name;
					absolutePath += FILE_SEPARATOR;
				}
				int id_shell = get_active_thread_by_type(SHELL);
				int return_value = change_thread_current_folder(id_shell, &(std::string(absolutePath)));
				return !return_value;
			}
			else {
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
	SetLastError(errorBadPath);
	std::cout << "BAD PATH\n";
	return false;
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

bool containColon(std::string str)
{
	size_t a = str.find(':');
	if (a != std::string::npos) {
		return true;
	}
	return false;
}

std::string getAbsolutePathFromRelative(int procesId, std::string relativePath)
{
	std::string workDirectory = get_thread_current_folder(procesId);

	return std::string(workDirectory + relativePath);
}

/*
 * @return FileDescriptor of new created FileDescriptor
*/
THandle putFileIntoFDTable(File* file, size_t flags) {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	FileDescriptorBlock* newFD = new FileDescriptorBlock(file->path, flags, file);
	FileDescriptorTable.push_back(newFD);

	return (THandle)newFD->getId();
}

File* removeFileFromFDTable(THandle fileDescriptor) {
	size_t id = (size_t)fileDescriptor;
	for (size_t i = 0; i < FileDescriptorTable.size(); i++)
	{
		if (FileDescriptorTable[i]->getId() == id) {
			File* file = FileDescriptorTable[i]->filePointer;
			if (getStdIn() != fileDescriptor && getStdOut() != fileDescriptor) {
				FileDescriptorTable.erase(FileDescriptorTable.begin() + i);
			}
			return file;
		}
	}
	return nullptr;
}

bool canRead(THandle fileDescriptor)
{
	size_t id = (size_t)fileDescriptor;
	for (FileDescriptorBlock* f : FileDescriptorTable) {
		if (f->getId() == id) {
			if (f->flags & FILE_SHARE_READ) {
				return true;
			}
			else if (f->flags & GENERIC_READ) {
				return true;
			}
			else {
				return false;
			}
		}

	}
	return false;
}

bool canWrite(THandle fileDescriptor)
{
	size_t id = (size_t)fileDescriptor;
	for (FileDescriptorBlock* f : FileDescriptorTable) {
		if (f->getId() == id) {
			if (f->flags & FILE_SHARE_WRITE) {
				return true;
			}
			if (f->flags & GENERIC_WRITE) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	return false;
}

bool canOpen(std::string fullFilePath, size_t flags)
{
	for (FileDescriptorBlock *f : FileDescriptorTable) {
		if (f->filePointer->path == fullFilePath) {
			if ((flags & FILE_SHARE_WRITE) || flags & GENERIC_WRITE) {
				if (f->flags & GENERIC_WRITE) {
					return false;
				}
			}
			if ((flags & FILE_SHARE_READ) || flags & GENERIC_READ) {
				if (f->flags & GENERIC_READ) {
					return false;
				}
			}
		}
	}
	return true;
}


File* getFileByTHandle(THandle fileDescriptor) {
	size_t id = (size_t)fileDescriptor;

	for (FileDescriptorBlock *f : FileDescriptorTable)
	{
		if (f->getId() == id) {
			return f->filePointer;
		}
	}
	return nullptr;
}

void init()
{
	std::string path_in = ROOT_FOLDER;
	path_in += FILE_SEPARATOR;
	path_in += "std_in";
	std::string path_out = ROOT_FOLDER;
	path_out += FILE_SEPARATOR;
	path_out += "std_out";;
	Stdin* std_in = new Stdin("std_in", path_in);
	Stdout* std_out = new Stdout("std_out", path_out);

	FileDescriptorBlock* std_inFD = new FileDescriptorBlock(std_in->path, FILE_SHARE_READ, std_in);
	FileDescriptorBlock* std_outFD = new FileDescriptorBlock(std_in->path, FILE_SHARE_WRITE, std_out);

	FileDescriptorTable.push_back(std_inFD);
	FileDescriptorTable.push_back(std_outFD);
}

THandle getStdIn() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	THandle std_in = (THandle)0;

	return std_in;
}

bool createPipe(THandle* input, THandle *output)
{
	Pipe* newPipe = new Pipe("New-Pipe");
	*input = putFileIntoFDTable(newPipe, GENERIC_WRITE);
	*output = putFileIntoFDTable(newPipe, GENERIC_READ);

	return true;
}

THandle getStdOut() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}

	THandle std_out = (THandle)1;

	return std_out;
}