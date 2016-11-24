#include "fs.h"
#include <iostream>
#include <typeinfo>

#include "File.h"
#include "FileDescriptorBlock.h"
#include "stdin.h"
#include "stdout.h"
#include "pipe.h"
#include "TCB.h"

/* Ko�enov� adres�� filesyst�mu */
Folder* rootFolder = new Folder(ROOT_FOLDER, nullptr);
/* Tabulka filedescriptor� */
std::vector<FileDescriptorBlock*> FileDescriptorTable;

/*
 * Otev�r� soubor a vrac� na File-Descriptor, kter� ho reprezentuje
 */
THandle openFile(int procesId, std::string fullFilePath, size_t flags)
{
	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return nullptr;

	// Cesta se prohled�v� od ko�enov�ho adres��e
	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFile(partsOfPath[i])) {
				File *foundedFile = tmpFolder->getFileByName(partsOfPath[i]);
				// Test zda je mo�n� soubor otev��t
				if (!canOpen(fullFilePath, flags)) {
					SetLastError(errorFileIsUsed);
					return nullptr;
				}
				foundedFile->setOpened();
				// Z�sk�n� filedescriptoru
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
 * Vytv��� soubor a vrac� na File-Descriptor, kter� ho reprezentuje. V p��pad�, �e soubor existuje nastav� chybu a op�t vr�t� File-Descriptor, kter� ho reprezentuje.
 */
THandle createFile(int procesId, std::string fullFilePath, size_t flags)
{
	// Z�sk�n�/vytvo�en� standardn�ho vstupu/v�stupu
	if (fullFilePath == "CONOUT$") {
		return getStdOut();
	}
	else if (fullFilePath == "CONIN$") {
		return getStdIn();
	}

	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return nullptr;

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			// test, zda n�zev souboru obsahuje dvojte�ku
			if (containColon(partsOfPath[i])) {
				SetLastError(errorBadPath);
				return nullptr;
			}
			// test, zda posledn� slo�ka ji� neobsahuje soubor
			if (tmpFolder->containFile(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				// Nalezen� soubor
				File *foundFile = tmpFolder->getFileByName(partsOfPath[i]);
				if (!canOpen(fullFilePath, flags)) {
					SetLastError(errorFileIsUsed);

					return nullptr;
				}
				foundFile->setOpened();
				// Vr�t� se file descriptor nalezen�ho souboru
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
				// Vytvo�en� nov�ho souboru a vr�cen� filedescriptoru
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
 * Zapisuje obsah do souboru, podle nastaven�ho flagu
 */
int writeFile(THandle file, std::string buffer, size_t flag)
{
	// Z�sk�n� souboru z tabulky filedescriptor�
	File *tmpFile = getFileByTHandle(file);
	if (canWrite(file)) {
		int numberOfBytes = (int)tmpFile->write(buffer, flag);
		// o�et�en� chybov�ho stavu otev�en�ho souboru
		if (numberOfBytes < 0) {
			SetLastError(errorFileIsUsed);
		}
		// Vrac� po�et zapsan�ch znak�/byt�
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
 * P��pis obsahu do souboru
 */
int appendFile(THandle file, std::string buffer)
{
	// Z�sk�n� souboru z tabulky filedescriptor�
	File *tmpFile = getFileByTHandle(file);
	if (canWrite(file)) {
		int numberOfBytes = (int)tmpFile->append(buffer);
		// o�et�en� chybov�ho stavu otev�en�ho souboru
		if (numberOfBytes < 0) {
			SetLastError(errorFileIsUsed);
		}
		// Vrac� po�et zapsan�ch znak�/byt�
		return numberOfBytes;
	}

	SetLastError(errorFilePermission);
	return -1;
}

/*
 * P�e�te a vr�t� obsah souboru
 */
std::string readFile(THandle file)
{
	// Z�sk�n� souboru z tabulky filedescriptor�
	File *tmpFile = getFileByTHandle(file);
	if (canRead(file)) {
		std::string str(tmpFile->read());
		return str;
	}

	SetLastError(errorFilePermission);
	return nullptr;
}

/*
 * Vrac� true p�i �sp�n�m zav�en� souboru
 */
bool closeFile(THandle file)
{
	// Odstran� a vr�t� soubor z tabulky filedescriptor�
	File *tmpFile = removeFileFromFDTable(file);
	if (tmpFile == NULL) {
		return false;
	}
	// nastav� flag souboru na closed
	return tmpFile->setClosed();
}

/*
 * Vytvo�� slo�ku a vr�t� File-Descriptor
 */
bool createFolder(int procesId, std::string fullFolderPath)
{
	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;
	// Postupn� proch�z� cestu od ko�enov�ho adres��e
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			// Test, jestli slo�ka ji� neexistuje
			if (tmpFolder->containFolder(partsOfPath[i])) {
				SetLastError(errorAlreadyExist);
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);

				return false;
			}
			else {
				Folder* newFolder = new Folder(partsOfPath[i], tmpFolder);
				// P�id� nov� vytvo�enou slo�ku do rodi�ovsk� slo�ky
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
 * Sma�e slo�ku podle cesty
 */
bool deleteFolderByPath(int procesId, std::string fullFolderPath)
{
	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;

	for (int i = 1; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				// Sma�e nalezenou slo�ku a vr�t� v�sledek
				bool result = false;
				// Test, zda je slo�ka zam�ena proti smaz�n�
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
 * Zamyk� slo�ku proti p��padn�mu smaz�n�
 */
bool lockFolder(std::string fullFolderPath)
{
	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	if (partsOfPath.size() == 0 || !containRoot(fullFolderPath))
		return false;

	Folder *tmpFolder = rootFolder;
	// Proch�z� cestu od ko�enov�ho adres��e
	for (int i = 0; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			// Test, zda je soubor nalezen
			if (tmpFolder->name == partsOfPath[i]) {
				// Zam�en� souboru a vr�cen� v�sledn� hodnoty
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
 * Odemyk� zam�enou slo�ku - p�i �sp�n�m odem�en� vrac� true
 */
bool unLockFolder(std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	if (partsOfPath.size() == 0 || !containRoot(fullFolderPath))
		return false;

	Folder *tmpFolder = rootFolder;

	for (int i = 0; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->name == partsOfPath[i]) {
				// Zkus� odemknout z�mek proti smaz�n� slo�ky
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
 * Ma�e soubor podle cesty k n�mu
 */
bool deleteFileByPath(int procesId, std::string fullFilePath) {
	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFilePath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;
	for (int i = 1; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
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
 * Ma�e soubor podle File-Descriptoru
 */
bool deleteFile(int procesId, THandle file)
{
	File *tmpFile = getFileByTHandle(file);
	std::string path = tmpFile->path;
	return deleteFileByPath(procesId, path);
}

/*
 * Parsuje cestu - vrac� vektor s ��stmi cesty (soubory a slo�ky)
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
 * Upravuje ��sti cesty, kdy� obsahuje symboly pro sou�asnou (.) a nad�azenou (..) slo�ku
 */
std::vector<std::string> correctPath(std::vector<std::string> pathParts)
{
	// Test kdy� je za ko�enov�m adres��em symbol pro nad�azenou slo�ku -> chyba
	if (pathParts.size() > 1) {
		if (pathParts[1] == UPPER_DIRECTORY) {
			SetLastError(errorBadPath);
			return std::vector<std::string>();
		}
	}
	// Uprava cesty pro nad�azenou slo�ku '..'
	for (int i = 1; i < (pathParts.size() - 1); i++) {
		if (pathParts[i + 1] == UPPER_DIRECTORY) {
			pathParts.erase(pathParts.begin() + (i + 1));
			pathParts.erase(pathParts.begin() + i);
			i -= 2;
		}
	}
	// Uprava cesty pro sou�asnou slo�ku '.'
	for (int i = 0; i < (pathParts.size() - 1); i++) {
		if (pathParts[i + 1] == CURRENT_DIRECTORY) {
			pathParts.erase(pathParts.begin() + (i + 1));
		}
	}
	return pathParts;
}

/*
 * Kontroluuje spr�vnost cesty a v p��pad� validn� cesty vrac� vektor s �astmi cesty
 */
std::vector<std::string> checkPath(int proces_id, std::string fullPath)
{
	std::vector<std::string> partsOfPath;
	if (!containRoot(fullPath)) {
		// Z�sk�n� absolutn� cesty
		fullPath = getAbsolutePathFromRelative(proces_id, fullPath);
		// Parsov�n� absolutn� cesty -> ��sti cesty
		partsOfPath = parsePath(fullPath);
		// Upraven� cesty -> odstran�n� symbol� pro nad�ezenou a sou�asnou slo�ku
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
	// Vrac� ji� vektor s ��stmi cesty
	return partsOfPath;
}

/*
 * Vrac� �et�zec pro v�pis obsahu slo�ky - zejm�na pro proces DIR
 */
std::string printFolder(int procesId, std::string fullFolderPath)
{
	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;


	Folder *tmpFolder = rootFolder;

	for (int i = 0; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->name == partsOfPath[i]) {
				// Vrac� �et�zec v�pisu obsahu slo�ky
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
 * M�n� pracovn� adres�� proces� podle jejich id
 */
bool changeWorkDirectory(int procesId, std::string fullFolderPath)
{
	// z�sk�n� ��st� cesty k souboru
	std::vector<std::string> partsOfPath = checkPath(procesId, fullFolderPath);
	if (partsOfPath.size() == 0)
		return false;

	Folder *tmpFolder = rootFolder;
	// v p��pad�, kdy� je po�adovan� slo�ka ko�enov� adres��
	if (partsOfPath.size() == 1) {
		int id_shell = get_active_thread_by_type(SHELL);
		std::string str = ROOT_FOLDER;
		str += FILE_SEPARATOR;
		int return_value = change_thread_current_folder(id_shell, &(std::string(str)));
		return !return_value;
	}

	for (int i = 1; i < partsOfPath.size(); i++) {
		// Kdy� naraz� na posledn� polo�ku cesty
		if (i == (partsOfPath.size() - 1)) {
			if (tmpFolder->containFolder(partsOfPath[i])) {
				Folder *foundFolder = tmpFolder->getFolderByName(partsOfPath[i]);
				// Sestaven� nov�ho pracovn�ho adres��e procesu
				std::string absolutePath = "";
				for (std::string name : partsOfPath) {
					absolutePath += name;
					absolutePath += FILE_SEPARATOR;
				}
				// Z�sk�n� id shellu
				int id_shell = get_active_thread_by_type(SHELL);
				// nastaven� pracovn�ho adres��e aktu�ln� aktivn�ho shelu
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
 * Testuje, zda cesta obsahuje ko�enov� adres��
 */
bool containRoot(std::string fullFolderPath)
{
	std::vector<std::string> partsOfPath = parsePath(fullFolderPath);
	// Jednoduch� test, zda cesta obsahuje ko�enov� adres�� a to na prvn�m m�st� cesty
	if (partsOfPath.size() > 0) {
		if (partsOfPath[0] == ROOT_FOLDER) {
			return true;
		}
	}
	return false;

}

/*
 * Testuje, zda cesta neobsahuje nevalidn� znak ':'
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
 * Z�sk� absolutn� cestu za pomoci relativn� cesty
 */
std::string getAbsolutePathFromRelative(int procesId, std::string relativePath)
{
	std::string workDirectory = get_thread_current_folder(procesId);
	// Funkce po��t� s t�m, �e parametr relativePath je opravdu relativn� cesta
	return std::string(workDirectory + relativePath);
}

/*
 * Vlo�� nov� soubor do tabulky filedescriptor� a vrac� filedescriptor, kter� bude soubor reprezentovat
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
 * Sma�e soubor z tabulky filedescriptor� a vr�t� ho
 */
File* removeFileFromFDTable(THandle fileDescriptor) {
	size_t id = (size_t)fileDescriptor;
	for (size_t i = 0; i < FileDescriptorTable.size(); i++)
	{
		if (FileDescriptorTable[i]->getId() == id) {
			// Vrac� pointer na soubor, kter� je ulo�en ve FileDescriptorBloku
			File* file = FileDescriptorTable[i]->filePointer;
			if (getStdIn() != fileDescriptor && getStdOut() != fileDescriptor) {
				// Samotn� smaz�n� z filedescriptor tabulky
				FileDescriptorTable.erase(FileDescriptorTable.begin() + i);
			}
			return file;
		}
	}
	return nullptr;
}

/*
 * Test, zda je mo�n� soubor ��st
 */
bool canRead(THandle fileDescriptor)
{
	size_t id = (size_t)fileDescriptor;
	for (FileDescriptorBlock* f : FileDescriptorTable) {
		if (f->getId() == id) {
			// Test, zda flags obsahuj� sd�len� �ten�
			if (f->flags & FILE_SHARE_READ) {
				return true;
			}
			// Test, zda flags obsahuj� obecn� �ten�
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
 * Test, zda je mo�n� do souboru zapisovat
 */
bool canWrite(THandle fileDescriptor)
{
	size_t id = (size_t)fileDescriptor;
	for (FileDescriptorBlock* f : FileDescriptorTable) {
		if (f->getId() == id) {
			// Test, zda flags obsahuj� sd�len� z�pis
			if (f->flags & FILE_SHARE_WRITE) {
				return true;
			}
			// Test, zda flags obsahuj� obecn� z�pis
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
 * Test, zda je mo�n� soubor otev��t
 */
bool canOpen(std::string fullFilePath, size_t flags)
{
	for (FileDescriptorBlock *f : FileDescriptorTable) {
		if (f->filePointer->path == fullFilePath) {
			// Test, zda flags obsahuj� sd�len� nebo obecn� z�pis
			if ((flags & FILE_SHARE_WRITE) || flags & GENERIC_WRITE) {
				// kdy� u� m� soubor nastaven� flag obecn�ho z�pisu, nen� mo�n� ho znovu pro z�pis otev��t
				if (f->flags & GENERIC_WRITE) {
					return false;
				}
			}
			// Test, zda flags obsahuj� sd�len� nebo obecn� �ten�
			if ((flags & FILE_SHARE_READ) || flags & GENERIC_READ) {
				// kdy� u� m� soubor nastaven� flag obecn�ho �ten�, nen� mo�n� ho znovu pro �ten� otev��t
				if (f->flags & GENERIC_READ) {
					return false;
				}
			}
		}
	}
	return true;
}

/*
 * Z�sk� soubor z tabulky filedescriptor� a vr�t� ho
 */
File* getFileByTHandle(THandle fileDescriptor) {
	size_t id = (size_t)fileDescriptor;

	for (FileDescriptorBlock *f : FileDescriptorTable)
	{
		if (f->getId() == id) {
			// Vrac� pointer na soubor -> s t�mto pointerem se pracuje pouze ve FS
			return f->filePointer;
		}
	}
	return nullptr;
}


/*
* Vrac� standardn� v�stup, kter� se chov� jako soubor
*/
THandle getStdOut() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}

	THandle std_out = (THandle)1;

	return std_out;
}

/*
* Vrac� standardn� vstup, kter� se chov� jako soubor
*/
THandle getStdIn() {
	if (FileDescriptorTable.size() == 0) {
		init();
	}
	THandle std_in = (THandle)0;

	return std_in;
}

/*
 * Funkce, kter� inicializuje pot�ebn� soubory - vytvo�� v tabulce filedescriptor� descriptory pro std_in a std_out
 */
void init()
{
	// Soubory sice maj� cestu v ko�enov�m adres��i ale p��mo se k n�mu nep�ipoj� (nejsou vid�t v root adres��i) -> simulace neviditeln�ho souboru
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

	// Vlo�en� do tabulky filedescriptor�
	FileDescriptorTable.push_back(std_inFD);
	FileDescriptorTable.push_back(std_outFD);
}

/*
 * Vytvo�� rouru a parametry definuje jako vstup a v�stup roury
 */
bool createPipe(THandle* input, THandle *output)
{
	Pipe* newPipe = new Pipe("New-Pipe");
	// Roura neexistuje v adres��ov�m strom�, ale tv��� se jako soubor (volaj� se nad n�m stejn� metody)
	*input = putFileIntoFDTable(newPipe, GENERIC_WRITE);
	*output = putFileIntoFDTable(newPipe, GENERIC_READ);

	return true;
}
