#pragma once

#include "FileDescriptor.h"

#include <vector>
#include <string>
#include <mutex>
#include "File.h"

/*
 * Objekty t��dy Folder reprezentuj� jednotliv� slo�ky Filesystemu.
 * D�d� od t��dy FileDescriptor
 */
class Folder : public FileDescriptor
{
public:
	/* Vektor pointr� na soubory, kter� slo�ka obsahuje */
	std::vector<File* > files;
	/* Vektor pointr� na slo�ky, kter� slo�ka obsahuje */
	std::vector<Folder* > folders;
	/* Pointer na rodi�ovskou slo�ku */
	Folder* parentFolder;
	/* Z�mek proti p��padn�mu smaz�n� slo�ky */
	int lockCounter;

	/* Mutex pro lockCounter */
	std::mutex lockCounterMtx;
	/* Mutex pro vektor soubor� */
	std::mutex filesMtx;
	/* Mutex pro vektor slo�ek */
	std::mutex foldersMtx;

	/* P�id�v� nov� podsoubor */
	bool addFile(File* file);
	/* P�id�v� novou podslo�ku */
	bool addFolder(Folder* file);

	/* Odebere podsoubor podle jm�na, kdy� nen� otev�en�. Jinak vrac� false */
	bool removeFile(std::string name);
	/* Odebere podslo�ku podle jm�na.Prov�d� rekurzivn� proch�zen� podslo�ek a postupn� maz�n�. Kdy� naraz� na otev�en� soubor nesma�e pouze cestu k tomuto souboru.*/
	bool removeFolder(std::string name);

	/* Vruc� true, kdy� slo�ka obsahuje soubor se jm�nem */
	bool containFile(std::string name);
	/* Vruc� true, kdy� slo�ka obsahuje slo�ku se jm�nem */
	bool containFolder(std::string name);

	/* Zamyk� slo�ku */
	bool lockFolder();
	/* Odemyk� slo�ku */
	bool unLockFolder();
	/* Testuje, jestli je slo�ka zam�en� true = zam�eno */
	bool isLock();

	/* Vrac� podsoubor slo�ky podle jm�na */
	File* Folder::getFileByName(std::string name);
	/* Vrac� podslo�ku slo�ky podle jm�na */
	Folder* Folder::getFolderByName(std::string name);

	/* Vyp�e obsah slo�ky */
	std::string printChildren();

	/* Konstruktor */
	Folder(std::string name, Folder* parent);
	/* Destruktor */
	~Folder();
};