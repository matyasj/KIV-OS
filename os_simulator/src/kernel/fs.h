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

/* Vytvoøí složku a v pøípadì úspìchu vrátí true */
bool createFolder(int procesId, std::string fullFolderPath);
/* Smaže složku podle cesty */
bool deleteFolderByPath(int procesId, std::string fullFolderPath);

/* Zamyká složku proti pøípadnému smazání */
bool lockFolder(std::string fullFolderPath);
/* Odemyká složku */
bool unLockFolder(std::string fullFolderPath);

/* Parsuje cestu - vrací vektor s èástmi cesty (soubory a složky) */
std::vector<std::string> parsePath(std::string path);
/* Upravuje èásti cesty, když obsahuje symboly pro souèasnou (.) a nadøazenou (..) složku */
std::vector<std::string> correctPath(std::vector<std::string> pathParts);
/* Kontroluuje správnost cesty a v pøípadì validní cesty vrací vektor s èastmi cesty */
std::vector<std::string> checkPath(int proces_id, std::string fullPath);

/* Vrací øetìzec pro výpis obsahu složky - zejména pro proces DIR */
std::string printFolder(int procesId, std::string fullFolderPath);
/* Mìní pracovní adresáø procesù podle jejich id */
bool changeWorkDirectory(int procesId, std::string fullFolderPath);

/* Testuje, zda cesta obsahuje koøenový adresáø */
bool containRoot(std::string fullFolderPath);
/* Testuje, zda cesta neobsahuje nevalidní znak ':'  */
bool containColon(std::string str);
/* Získá absolutní cestu za pomoci relativní cesty */
std::string getAbsolutePathFromRelative(int procesId, std::string relativePath);

/* Vloží nový soubor do tabulky filedescriptorù a vrací filedescriptor, který bude soubor reprezentovat */
THandle putFileIntoFDTable(File* file, size_t flags);
/* Získá soubor z tabulky filedescriptorù a vrátí ho */
File* getFileByTHandle(THandle fileDescriptor);
/* Smaže soubor z tabulky filedescriptorù a vrátí ho */
File* removeFileFromFDTable(THandle fileDescriptor);

/* Test, zda je možné soubor èíst */
bool canRead(THandle fileDescriptor);
/* Test, zda je možné do souboru zapisovat */
bool canWrite(THandle fileDescriptor);
/* Test, zda je možné soubor otevøít */
bool canOpen(std::string fullFilePath, size_t flags);

/* Vrací standardní výstup, který se chová jako soubor */
THandle getStdOut();
/* Vrací standardní vstup, který se chová jako soubor */
THandle getStdIn();
/* Funkce, která inicializuje potøebné soubory - vytvoøí v tabulce filedescriptorù descriptory pro std_in a std_out */
void init();

/* Vytvoøí rouru a parametry definuje jako vstup a výstup roury */
bool createPipe(THandle* input, THandle *output);