#pragma once

#include "FileDescriptor.h"

#include <vector>
#include <string>
#include <mutex>
#include "File.h"

/*
 * Objekty tøídy Folder reprezentují jednotlivé složky Filesystemu.
 * Dìdí od tøídy FileDescriptor
 */
class Folder : public FileDescriptor
{
public:
	/* Vektor pointrù na soubory, které složka obsahuje */
	std::vector<File* > files;
	/* Vektor pointrù na složky, které složka obsahuje */
	std::vector<Folder* > folders;
	/* Pointer na rodièovskou složku */
	Folder* parentFolder;
	/* Zámek proti pøípadnému smazání složky */
	int lockCounter;

	/* Mutex pro lockCounter */
	std::mutex lockCounterMtx;
	/* Mutex pro vektor souborù */
	std::mutex filesMtx;
	/* Mutex pro vektor složek */
	std::mutex foldersMtx;

	/* Pøidává nový podsoubor */
	bool addFile(File* file);
	/* Pøidává novou podsložku */
	bool addFolder(Folder* file);

	/* Odebere podsoubor podle jména, když není otevøený. Jinak vrací false */
	bool removeFile(std::string name);
	/* Odebere podsložku podle jména.Provádí rekurzivní procházení podsložek a postupné mazání. Když narazí na otevøený soubor nesmaže pouze cestu k tomuto souboru.*/
	bool removeFolder(std::string name);

	/* Vrucí true, když složka obsahuje soubor se jménem */
	bool containFile(std::string name);
	/* Vrucí true, když složka obsahuje složku se jménem */
	bool containFolder(std::string name);

	/* Zamyká složku */
	bool lockFolder();
	/* Odemyká složku */
	bool unLockFolder();
	/* Testuje, jestli je složka zamèená true = zamèeno */
	bool isLock();

	/* Vrací podsoubor složky podle jména */
	File* Folder::getFileByName(std::string name);
	/* Vrací podsložku složky podle jména */
	Folder* Folder::getFolderByName(std::string name);

	/* Vypíše obsah složky */
	std::string printChildren();

	/* Konstruktor */
	Folder(std::string name, Folder* parent);
	/* Destruktor */
	~Folder();
};