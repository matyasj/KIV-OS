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

/* Otev�r� soubor a vrac� na File-Descriptor, kter� ho reprezentuje */
THandle openFile(int procesId, std::string fullFilePath, size_t flags);
/* Vytv��� soubor a vrac� na File-Descriptor, kter� ho reprezentuje. V p��pad�, �e soubor existuje nastav� chybu a op�t vr�t� File-Descriptor, kter� ho reprezentuje. */
THandle createFile(int procesId, std::string fullFilePath, size_t flags);

/* Zapisuje obsah do souboru, podle nastaven�ho flagu */
int writeFile(THandle file, std::string buffer, size_t flag);
/* Nastavuje novou pozici v souboru */
bool setInFilePosition(THandle file, int newPosition);
/* P��pis obsahu do souboru */
int appendFile(THandle file, std::string buffer);
/* P�e�te a vr�t� obsah souboru */
std::string readFile(THandle file);
/* Vrac� true p�i �sp�n�m zav�en� souboru */
bool closeFile(THandle file);
/* Ma�e soubor podle cesty k n�mu */
bool deleteFileByPath(int procesId, std::string fullFilePath);
/* Ma�e soubor podle File-Descriptoru */
bool deleteFile(int procesId, THandle file);

/* Vytvo�� slo�ku a vr�t� File-Descriptor */
THandle createFolder(int procesId, std::string fullFolderPath);
/* Sma�e slo�ku podle cesty */
bool deleteFolderByPath(int procesId, std::string fullFolderPath);
/* Sma�e soubor podle File-Descriptoru */
bool deleteFolder(int procesId, THandle folder);

/* Zamyk� slo�ku proti p��padn�mu smaz�n� */
bool lockFolder(std::string fullFolderPath);
/* Odemyk� slo�ku */
bool unLockFolder(std::string fullFolderPath);

/* Parsuje cestu - vrac� vektor s ��stmi cesty (soubory a slo�ky) */
std::vector<std::string> parsePath(std::string path);
/* Upravuje ��sti cesty, kdy� obsahuje symboly pro sou�asnou (.) a nad�azenou (..) slo�ku */
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