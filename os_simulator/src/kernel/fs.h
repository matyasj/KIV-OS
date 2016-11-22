#pragma once
#include <vector>
#include <string>
#include "..\common\api.h"
#include "FileDescriptor.h"
#include "Folder.h"


#define FILE_SEPARATOR '\\'
#define ROOT_FOLDER "C:"
#define UPPER_DIRECTORY ".."
#define CURRENT_DIRECTORY "."

/* Otevírá soubor a vrací na File-Descriptor, který ho reprezentuje */
THandle openFile(int procesId, std::string fullFilePath, size_t flags);
/* Vytváøí soubor a vrací na File-Descriptor, který ho reprezentuje. V pøípadì, že soubor existuje nastaví chybu a opìt vrátí File-Descriptor, který ho reprezentuje. */
THandle createFile(int procesId, std::string fullFilePath, size_t flags);

/* Zapisuje obsah do souboru, podle nastaveného flagu */
int writeFile(THandle file, std::string buffer, size_t flag);
/* Nastavuje novou pozici v souboru */
bool setInFilePosition(THandle file, int newPosition);
/* Pøípis obsahu do souboru */
int appendFile(THandle file, std::string buffer);
/* Pøeète a vrátí obsah souboru */
std::string readFile(THandle file);
/* Vrací true pøi úspìšném zavøení souboru */
bool closeFile(THandle file);
/* Maže soubor podle cesty k nìmu */
bool deleteFileByPath(int procesId, std::string fullFilePath);
/* Maže soubor podle File-Descriptoru */
bool deleteFile(int procesId, THandle file);

/* Vytvoøí složku a vrátí File-Descriptor */
THandle createFolder(int procesId, std::string fullFolderPath);
/* Smaže složku podle cesty */
bool deleteFolderByPath(int procesId, std::string fullFolderPath);
/* Smaže soubor podle File-Descriptoru */
bool deleteFolder(int procesId, THandle folder);

/* Zamyká složku proti pøípadnému smazání */
bool lockFolder(std::string fullFolderPath);
/* Odemyká složku */
bool unLockFolder(std::string fullFolderPath);

/* Parsuje cestu - vrací vektor s èástmi cesty (soubory a složky) */
std::vector<std::string> parsePath(std::string path);
/* Upravuje èásti cesty, když obsahuje symboly pro souèasnou (.) a nadøazenou (..) složku */
std::vector<std::string> correctPath(std::vector<std::string> pathParts);
/* Kontroluuje */
std::vector<std::string> checkPath(int proces_id, std::string fullPath);

std::string printFolder(int procesId, std::string fullFolderPath);
bool changeWorkDirectory(int procesId, std::string fullFolderPath);

bool containRoot(std::string fullFolderPath);
bool containColon(std::string str);
std::string getAbsolutePathFromRelative(int procesId, std::string relativePath);

// File Descriptors
THandle putFileIntoFDTable(File* file, size_t flags);
File* getFileByTHandle(THandle fileDescriptor);
File* removeFileFromFDTable(THandle fileDescriptor);

// Testy prav na cteni/zapis
bool canRead(THandle fileDescriptor);
bool canWrite(THandle fileDescriptor);
bool canOpen(std::string fullFilePath, size_t flags);


// Standartni vstupy/vystupy
THandle getStdOut();
THandle getStdIn();
void init(); // init metoda

// Create Pipe
bool createPipe(THandle* input, THandle *output);
//FileDescriptor getFileByName(char* fileName);