#include "fs.h"
#include <iostream>
#include <typeinfo>

#include "File.h"
#include "FileDescriptorBlock.h"
#include "stdin.h"
#include "stdout.h"
#include "pipe.h"
#include "TCB.h"

/* Koøenový adresáø filesystému */
Folder* rootFolder = new Folder(ROOT_FOLDER, nullptr);
/* Tabulka filedescriptorù */
std::vector<FileDescriptorBlock*> FileDescriptorTable;

/*
 * Otevírá soubor a vrací na File-Descriptor, který ho reprezentuje
 */
THandle openFile(int procesId, std::string fullFilePath, size_t flags)
{
	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return nullptr;

	// Cesta se prohledává od koøenového adresáøe
	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				File *foundedFile = tmpFolder->getFileByName(partsOfPath[i]);
				// Test zda je možné soubor otevøít
				if (!canOpen(fullFilePath, flags)) {
					SetLastError(errorFileIsUsed);
					return nullptr;
				}
				foundedFile->setOpened();
				// Získání filedescriptoru
				THandle newFileDescriptor = putFileIntoFDTable(foundedFile, flags);
				return newFileDescriptor;
			}
			else {
				SetLastError(errorFileNotFound);
				return nullptr;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
		}
	}
	SetLastError(errorFileNotFound);

	return nullptr;
}

/*
 * Vytváøí soubor a vrací na File-Descriptor, který ho reprezentuje. V pøípadì, že soubor existuje nastaví chybu a opìt vrátí File-Descriptor, který ho reprezentuje.
 */
THandle createFile(int procesId, std::string fullFilePath, size_t flags)
{
	// Získání/vytvoøení standardního vstupu/výstupu
	if (fullFilePath == "CONOUT$") {
		return getStdOut();
	}
	else if (fullFilePath == "CONIN$") {
		return getStdIn();
	}

	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return nullptr;

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			// test, zda název souboru obsahuje dvojteèku
			if (containColon(partsOfPath[i])) {
				SetLastError(errorBadPath);
				return nullptr;
			}
			// test, zda poslední složka již neobsahuje soubor
			if (tmpFolder->containFile(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				// Nalezený soubor
				File *foundFile = tmpFolder->getFileByName(partsOfPath[i]);
				if (!canOpen(fullFilePath, flags)) {
					SetLastError(errorFileIsUsed);

					return nullptr;
				}
				foundFile->setOpened();
				// Vrátí se file descriptor nalezeného souboru
				THandle newFileDescriptor = putFileIntoFDTable(foundFile, flags);
				if ((size_t)newFileDescriptor != -1) {
					return newFileDescriptor;
				}
				else {
					SetLastError(errorIO);
					return newFileDescriptor;
				}

			}
			else {
				// Vytvoøení nového souboru a vrácení filedescriptoru
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
			return nullptr;
		}
	}
	SetLastError(errorBadPath);
	return nullptr;
}

/*
 * Zapisuje obsah do souboru, podle nastaveného flagu
 */
int writeFile(THandle file, std::string buffer, size_t flag)
{
	// Získání souboru z tabulky filedescriptorù
	File *tmpFile = getFileByTHandle(file);
	if (canWrite(file)) {
		int numberOfBytes = (int)tmpFile->write(buffer, flag);
		// ošetøení chybového stavu otevøeného souboru
		if (numberOfBytes < 0) {
			SetLastError(errorFileIsUsed);
		}
		// Vrací poèet zapsaných znakù/bytù
		return numberOfBytes;
	}

	SetLastError(errorFilePermission);
	return -1;

}

/*
 * Nastavuje novou pozici v souboru
 */
bool setInFilePosition(THandle file, int newPosition)
{
	File *tmpFile = getFileByTHandle(file);
	return tmpFile->setPosition(newPosition);;
}

/*
 * Pøípis obsahu do souboru
 */
int appendFile(THandle file, std::string buffer)
{
	// Získání souboru z tabulky filedescriptorù
	File *tmpFile = getFileByTHandle(file);
	if (canWrite(file)) {
		int numberOfBytes = (int)tmpFile->append(buffer);
		// ošetøení chybového stavu otevøeného souboru
		if (numberOfBytes < 0) {
			SetLastError(errorFileIsUsed);
		}
		// Vrací poèet zapsaných znakù/bytù
		return numberOfBytes;
	}

	SetLastError(errorFilePermission);
	return -1;
}

/*
 * Pøeète a vrátí obsah souboru
 */
std::string readFile(THandle file)
{
	// Získání souboru z tabulky filedescriptorù
	File *tmpFile = getFileByTHandle(file);
	if (canRead(file)) {
		std::string str(tmpFile->read());
		return str;
	}

	SetLastError(errorFilePermission);
	return nullptr;
}

/*
 * Vrací true pøi úspìšném zavøení souboru
 */
bool closeFile(THandle file)
{
	// Odstraní a vrátí soubor z tabulky filedescriptorù
	File *tmpFile = removeFileFromFDTable(file);
	if (tmpFile == NULL) {
		return false;
	}
	// nastaví flag souboru na closed
	return tmpFile->setClosed();
}

/*
 * Vytvoøí složku a vrátí File-Descriptor
 */
bool createFolder(int procesId, std::string fullFolderPath)
{
	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;
	// Postupnì prochází cestu od koøenového adresáøe
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			// Test, jestli složka již neexistuje
			if (tmpFolder->containFolder(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);

				return false;
			}
			else {
				Folder* newFolder = new Folder(partsOfPath[i], tmpFolder);
				// Pøidá novì vytvoøenou složku do rodièovské složky
				tmpFolder->addFolder(newFolder);

				return true;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			return false;
		}
	}
	return false;
}

/*
 * Smaže složku podle cesty
 */
bool deleteFolderByPath(int procesId, std::string fullFolderPath)
{
	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				// Smaže nalezenou složku a vrátí výsledek
				bool result = false;
				// Test, zda je složka zamèena proti smazání
				if (!tmpFolder->getFolderByName(partsOfPath[i])->isLock()) {
					result = tmpFolder->removeFolder(partsOfPath[i]);
				}
				else {
					SetLastError(folderIsLock);
				}

				return result;
			}
			else {
				SetLastError(errorFileNotFound);
				return false;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			return false;
		}
	}
	SetLastError(errorFileNotFound);

	return false;
}

/*
 * Zamyká složku proti pøípadnému smazání
 */
bool lockFolder(std::string fullFolderPath)
{
	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	if (partsOfPath.size() == 0 || !containRoot(fullFolderPath))
		return false;

	Folder *tmpFolder = rootFolder;
	// Prochází cestu od koøenového adresáøe
	for (int i = 0; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			// Test, zda je soubor nalezen
			if (tmpFolder->name == partsOfPath[i]) {
				// Zamèení souboru a vrácení výsledné hodnoty
				return tmpFolder->lockFolder();
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i + 1])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i + 1]);
		}
		else {
			SetLastError(errorBadPath);
			return false;
		}
	}
	SetLastError(errorFileNotFound);

	return false;
}

/*
 * Odemyká zamèenou složku - pøi úspìšném odemèení vrací true
 */
bool unLockFolder(std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	if (partsOfPath.size() == 0 || !containRoot(fullFolderPath))
		return false;

	Folder *tmpFolder = rootFolder;

	for (int i = 0; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->name == partsOfPath[i]) {
				// Zkusí odemknout zámek proti smazání složky
				if (tmpFolder->unLockFolder()) {
					return true;
				}
				SetLastError(folderIsLock);
				return false;

			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i + 1])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i + 1]);
		}
		else {
			SetLastError(errorBadPath);
			return false;
		}
	}
	SetLastError(folderIsLock);

	return false;
}

/*
 * Maže soubor podle cesty k nìmu
 */
bool deleteFileByPath(int procesId, std::string fullFilePath) {
	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				return tmpFolder->removeFile(partsOfPath[i]);
			}
			else {
				SetLastError(errorFileNotFound);
				return false;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			return false;
		}
	}
	SetLastError(errorFileNotFound);
	return false;
}

/*
 * Maže soubor podle File-Descriptoru
 */
bool deleteFile(int procesId, THandle file)
{
	File *tmpFile = getFileByTHandle(file);
	std::string path = tmpFile->path;
	return deleteFileByPath(procesId, path);
}

/*
 * Parsuje cestu - vrací vektor s èástmi cesty (soubory a složky)
 */
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

/*
 * Upravuje èásti cesty, když obsahuje symboly pro souèasnou (.) a nadøazenou (..) složku
 */
std::vector<std::string> correctPath(std::vector<std::string> pathParts)
{
	// Test když je za koøenovým adresáøem symbol pro nadøazenou složku -> chyba
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

/*
 * Kontroluuje správnost cesty a v pøípadì validní cesty vrací vektor s èastmi cesty
 */
std::vector<std::string> checkPath(int proces_id, std::string fullPath)
{
	std::vector<std::string> partsOfPath;
	if (!containRoot(fullPath)) {
		// Získání absolutní cesty
		fullPath = getAbsolutePathFromRelative(proces_id, fullPath);
		// Parsování absolutní cesty -> èásti cesty
		partsOfPath = parsePath(fullPath);
		// Upravení cesty -> odstranìní symbolù pro nadøezenou a souèasnou složku
		partsOfPath = correctPath(partsOfPath);

		if (partsOfPath.size() == 0) {
			SetLastError(errorBadPath);
			return std::vector<std::string>();
		}

		if (partsOfPath[0] != ROOT_FOLDER) {
			SetLastError(errorBadPath);
			return std::vector<std::string>();
		}
	}
	else {
		partsOfPath = parsePath(fullPath);
		partsOfPath = correctPath(partsOfPath);
	}
	// Vrací již vektor s èástmi cesty
	return partsOfPath;
}

/*
 * Vrací øetìzec pro výpis obsahu složky - zejména pro proces DIR
 */
std::string printFolder(int procesId, std::string fullFolderPath)
{
	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;


	Folder *tmpFolder = rootFolder;

	for (int i = 0; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->name == partsOfPath[i]) {
				// Vrací øetìzec výpisu obsahu složky
				return tmpFolder->printChildren();
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i + 1])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i + 1]);
		}
		else {
			SetLastError(errorBadPath);
			return std::string();
		}
	}
	SetLastError(errorBadPath);
	return std::string();
}

/*
 * Mìní pracovní adresáø procesù podle jejich id
 */
bool changeWorkDirectory(int procesId, std::string fullFolderPath)
{
	// získání èástí cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;
	// v pøípadì, když je požadovaná složka koøenový adresáø
	if (partsOfPath.size() == 1) {
		int id_shell = get_active_thread_by_type(SHELL);
		std::string str = ROOT_FOLDER;
		str += FILE_SEPARATOR;
		int return_value = change_thread_current_folder(id_shell, &(std::string(str)));
		return !return_value;
	}

	for (int i = 1; i < partsOfPath.size(); i++) {
		// Když narazí na poslední položku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);
				// Sestavení nového pracovního adresáøe procesu
				std::string absolutePath = "";
				for (std::string name : partsOfPath) {
					absolutePath += name;
					absolutePath += FILE_SEPARATOR;
				}
				// Získání id shellu
				int id_shell = get_active_thread_by_type(SHELL);
				// nastavení pracovního adresáøe aktuálnì aktivního shelu
				int return_value = change_thread_current_folder(id_shell, &(std::string(absolutePath)));
				if (return_value == 0) {
					return true;
				}
				SetLastError(return_value);
				
				
			}
			else {
				SetLastError(errorBadPath);
				return false;
			}
		}
		else if (tmpFolder->containFolder(partsOfPath[i])) {
			tmpFolder = tmpFolder->getFolderByName(partsOfPath[i]);
		}
		else {
			SetLastError(errorBadPath);
			return false;
		}
	}
	SetLastError(errorBadPath);
	return false;
}

/*
 * Testuje, zda cesta obsahuje koøenový adresáø
 */
bool containRoot(std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	// Jednoduchý test, zda cesta obsahuje koøenový adresáø a to na prvním místì cesty
	if (partsOfPath.size() > 0) {
		if (partsOfPath[0] == ROOT_FOLDER) {
			return true;
		}
	}
	return false;

}

/*
 * Testuje, zda cesta neobsahuje nevalidní znak ':'
 */
bool containColon(std::string str)
{
	size_t a = str.find(':');
	if (a != std::string::npos) {
		return true;
	}
	return false;
}

/*
 * Získá absolutní cestu za pomoci relativní cesty
 */
std::string getAbsolutePathFromRelative(int procesId, std::string relativePath)
{
	std::string workDirectory = get_thread_current_folder(procesId);
	// Funkce poèítá s tím, že parametr relativePath je opravdu relativní cesta
	return std::string(workDirectory + relativePath);
}

/*
 * Vloží nový soubor do tabulky filedescriptorù a vrací filedescriptor, který bude soubor reprezentovat
 */
THandle putFileIntoFDTable(File* file, size_t flags) {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	FileDescriptorBlock* newFD = new FileDescriptorBlock(file->path, flags, file);
	FileDescriptorTable.push_back(newFD);

	return (THandle)newFD->getId();
}

/*
 * Smaže soubor z tabulky filedescriptorù a vrátí ho
 */
File* removeFileFromFDTable(THandle fileDescriptor) {
	size_t id = (size_t)fileDescriptor;
	for (size_t i = 0; i < FileDescriptorTable.size(); i++)
	{
		if (FileDescriptorTable[i]->getId() == id) {
			// Vrací pointer na soubor, který je uložen ve FileDescriptorBloku
			File* file = FileDescriptorTable[i]->filePointer;
			if (getStdIn() != fileDescriptor && getStdOut() != fileDescriptor) {
				// Samotné smazání z filedescriptor tabulky
				FileDescriptorTable.erase(FileDescriptorTable.begin() + i);
			}
			return file;
		}
	}
	return nullptr;
}

/*
 * Test, zda je možné soubor èíst
 */
bool canRead(THandle fileDescriptor)
{
	size_t id = (size_t)fileDescriptor;
	for (FileDescriptorBlock* f : FileDescriptorTable) {
		if (f->getId() == id) {
			// Test, zda flags obsahují sdílené ètení
			if (f->flags & FILE_SHARE_READ) {
				return true;
			}
			// Test, zda flags obsahují obecné ètení
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

/*
 * Test, zda je možné do souboru zapisovat
 */
bool canWrite(THandle fileDescriptor)
{
	size_t id = (size_t)fileDescriptor;
	for (FileDescriptorBlock* f : FileDescriptorTable) {
		if (f->getId() == id) {
			// Test, zda flags obsahují sdílený zápis
			if (f->flags & FILE_SHARE_WRITE) {
				return true;
			}
			// Test, zda flags obsahují obecný zápis
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

/*
 * Test, zda je možné soubor otevøít
 */
bool canOpen(std::string fullFilePath, size_t flags)
{
	for (FileDescriptorBlock *f : FileDescriptorTable) {
		if (f->filePointer->path == fullFilePath) {
			// Test, zda flags obsahují sdílený nebo obecný zápis
			if ((flags & FILE_SHARE_WRITE) || flags & GENERIC_WRITE) {
				// když už má soubor nastavený flag obecného zápisu, není možné ho znovu pro zápis otevøít
				if (f->flags & GENERIC_WRITE) {
					return false;
				}
			}
			// Test, zda flags obsahují sdílené nebo obecné ètení
			if ((flags & FILE_SHARE_READ) || flags & GENERIC_READ) {
				// když už má soubor nastavený flag obecného ètení, není možné ho znovu pro ètení otevøít
				if (f->flags & GENERIC_READ) {
					return false;
				}
			}
		}
	}
	return true;
}

/*
 * Získá soubor z tabulky filedescriptorù a vrátí ho
 */
File* getFileByTHandle(THandle fileDescriptor) {
	size_t id = (size_t)fileDescriptor;

	for (FileDescriptorBlock *f : FileDescriptorTable)
	{
		if (f->getId() == id) {
			// Vrací pointer na soubor -> s tímto pointerem se pracuje pouze ve FS
			return f->filePointer;
		}
	}
	return nullptr;
}


/*
* Vrací standardní výstup, který se chová jako soubor
*/
THandle getStdOut() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}

	THandle std_out = (THandle)1;

	return std_out;
}

/*
* Vrací standardní vstup, který se chová jako soubor
*/
THandle getStdIn() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	THandle std_in = (THandle)0;

	return std_in;
}

/*
 * Funkce, která inicializuje potøebné soubory - vytvoøí v tabulce filedescriptorù descriptory pro std_in a std_out
 */
void init()
{
	// Soubory sice mají cestu v koøenovém adresáøi ale pøímo se k nìmu nepøipojí (nejsou vidìt v root adresáøi) -> simulace neviditelného souboru
	std::string path_in = ROOT_FOLDER;
	path_in += FILE_SEPARATOR;
	path_in += "std_in";
	std::string path_out = ROOT_FOLDER;
	path_out += FILE_SEPARATOR;
	path_out += "std_out";

	Stdin* std_in = new Stdin("std_in", path_in);
	Stdout* std_out = new Stdout("std_out", path_out);

	FileDescriptorBlock* std_inFD = new FileDescriptorBlock(std_in->path, FILE_SHARE_READ, std_in);
	FileDescriptorBlock* std_outFD = new FileDescriptorBlock(std_in->path, FILE_SHARE_WRITE, std_out);

	// Vložení do tabulky filedescriptorù
	FileDescriptorTable.push_back(std_inFD);
	FileDescriptorTable.push_back(std_outFD);
}

/*
 * Vytvoøí rouru a parametry definuje jako vstup a výstup roury
 */
bool createPipe(THandle* input, THandle *output)
{
	Pipe* newPipe = new Pipe("New-Pipe");
	// Roura neexistuje v adresáøovém stromì, ale tváøí se jako soubor (volají se nad ním stejné metody)
	*input = putFileIntoFDTable(newPipe, GENERIC_WRITE);
	*output = putFileIntoFDTable(newPipe, GENERIC_READ);

	return true;
}
