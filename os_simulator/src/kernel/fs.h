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

/* Vytvo�� slo�ku a v p��pad� �sp�chu vr�t� true */
bool createFolder(int procesId, std::string fullFolderPath);
/* Sma�e slo�ku podle cesty */
bool deleteFolderByPath(int procesId, std::string fullFolderPath);

/* Zamyk� slo�ku proti p��padn�mu smaz�n� */
bool lockFolder(std::string fullFolderPath);
/* Odemyk� slo�ku */
bool unLockFolder(std::string fullFolderPath);

/* Parsuje cestu - vrac� vektor s ��stmi cesty (soubory a slo�ky) */
std::vector<std::string> parsePath(std::string path);
/* Upravuje ��sti cesty, kdy� obsahuje symboly pro sou�asnou (.) a nad�azenou (..) slo�ku */
std::vector<std::string> correctPath(std::vector<std::string> pathParts);
/* Kontroluuje spr�vnost cesty a v p��pad� validn� cesty vrac� vektor s �astmi cesty */
std::vector<std::string> checkPath(int proces_id, std::string fullPath);

/* Vrac� �et�zec pro v�pis obsahu slo�ky - zejm�na pro proces DIR */
std::string printFolder(int procesId, std::string fullFolderPath);
/* M�n� pracovn� adres�� proces� podle jejich id */
bool changeWorkDirectory(int procesId, std::string fullFolderPath);

/* Testuje, zda cesta obsahuje ko�enov� adres�� */
bool containRoot(std::string fullFolderPath);
/* Testuje, zda cesta neobsahuje nevalidn� znak ':'  */
bool containColon(std::string str);
/* Z�sk� absolutn� cestu za pomoci relativn� cesty */
std::string getAbsolutePathFromRelative(int procesId, std::string relativePath);

/* Vlo�� nov� soubor do tabulky filedescriptor� a vrac� filedescriptor, kter� bude soubor reprezentovat */
THandle putFileIntoFDTable(File* file, size_t flags);
/* Z�sk� soubor z tabulky filedescriptor� a vr�t� ho */
File* getFileByTHandle(THandle fileDescriptor);
/* Sma�e soubor z tabulky filedescriptor� a vr�t� ho */
File* removeFileFromFDTable(THandle fileDescriptor);

/* Test, zda je mo�n� soubor ��st */
bool canRead(THandle fileDescriptor);
/* Test, zda je mo�n� do souboru zapisovat */
bool canWrite(THandle fileDescriptor);
/* Test, zda je mo�n� soubor otev��t */
bool canOpen(std::string fullFilePath, size_t flags);

/* Vrac� standardn� v�stup, kter� se chov� jako soubor */
THandle getStdOut();
/* Vrac� standardn� vstup, kter� se chov� jako soubor */
THandle getStdIn();
/* Funkce, kter� inicializuje pot�ebn� soubory - vytvo�� v tabulce filedescriptor� descriptory pro std_in a std_out */
void init();

/* Vytvo�� rouru a parametry definuje jako vstup a v�stup roury */
bool createPipe(THandle* input, THandle *output);