#include "Folder.h"
#include <iostream>

/*
 * Pøidává nový podsoubor
 */
bool Folder::addFile(File* file)
{
	
	// Kontrola, zda již soubor se stejným jménem složka neobsahuje
	if (this->containFile(file->name)) {
		return false;
	}
	else {
		std::unique_lock<std::mutex> lck(filesMtx);
		this->files.push_back(file);
		file->parrentFolder = this;

		return true;
	}
}

/*
 * Pøidává novou podsložku
 */
bool Folder::addFolder(Folder* folder)
{
	// Kontrola, zda již složku se stejným jménem složka neobsahuje
	if (this->containFolder(folder->name)) {
		return false;
	}
	else {
		std::unique_lock<std::mutex> lck(foldersMtx);
		folder->parentFolder = this;
		this->folders.push_back(folder);
		return true;
	}
}

/*
 * Odebere podsoubor podle jména, když není otevøený. Jinak vrací false
 */
bool Folder::removeFile(std::string name)
{
	std::unique_lock<std::mutex> lck(filesMtx);
	// Prochází všechny podsoubory
	for (int i = 0; i < this->files.size(); i++) {
		// Když narazí na soubor s požadovaným jménem tak ho smaže
		if (this->files[i]->name == name) {
			//Kontrola, jestli není soubor otevøen
			if (this->files[i]->isOpened) {
				return false;
			}
			else {
				this->files.erase(this->files.begin() + i);
				return true;
			}
		}
	}
	return false;
}

/*
 * Odebere podsložku podle jména. Provádí rekurzivní procházení podsložek a postupné mazání.
 * Když narazí na otevøený soubor nesmaže pouze cestu k tomuto souboru.
 */
bool Folder::removeFolder(std::string name)
{
	std::unique_lock<std::mutex> lck(foldersMtx);
	bool result = true;
	for (int i = 0; i != this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			std::vector<Folder*>::iterator it = this->folders[i]->folders.begin();
			int numberOfOpenedFolders = 0;
			while (it != this->folders[i]->folders.end()) {
				bool locked = this->folders[i]->getFolderByName((*it)->name)->isLock();
				bool success = false;
				// Rekurzivní mazání podsložek
				if (!locked) {
					 success = this->folders[i]->removeFolder((*it)->name);
				}
				
				if (!success) {
					it++;
					numberOfOpenedFolders++;
					// Když jedna podsložka nebude smazána zùstane k ní celá cesta + test, zda je složka zamèená proti smazání
					result = (result && success && locked);
				}
				else {
					it = this->folders[i]->folders.begin();
					it += numberOfOpenedFolders;
					continue;
				}
			}

			// Mazani souborù
			std::vector<File*>::iterator fileIt = this->folders[i]->files.begin();
			int numberOfOpenedFiles = 0;
			while (fileIt != this->folders[i]->files.end()) {
				bool success = this->folders[i]->removeFile((*fileIt)->name);
				if (!success) {
					// Pøi nesmazání jednoho souboru (je stále otevøený) se zachová celá cesta složek k nìmu
					result = (result && success);
					fileIt++;
					numberOfOpenedFiles++;
				}
				else {
					fileIt = this->folders[i]->files.begin();
					fileIt += numberOfOpenedFiles;
					continue;
				}
			}
			if (result) {
				this->folders.erase(this->folders.begin() + i);
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
 * Vypíše obsah složky
 */
std::string Folder::printChildren()
{
	std::string printSTR = "Vypis adresare ";
	printSTR += this->name + "\n";
	foldersMtx.lock();
	for (int i = 0; i != this->folders.size(); i++) {
		printSTR += "|- <DIR>     " + this->folders[i]->name + "\n";
	}
	foldersMtx.unlock();
	filesMtx.lock();
	for (int i = 0; i != this->files.size(); i++) {
		printSTR += "|- <FILE>    " + this->files[i]->name + "\n";
	}
	filesMtx.unlock();
	return printSTR;
}

/*
 * Vrucí true, když složka obsahuje soubor se jménem
 */
bool Folder::containFile(std::string name)
{
	std::unique_lock<std::mutex> lck(filesMtx);
	// Prochází všechny podsoubory a kontroluje podle požadovaného jména
	for (int i = 0; i < this->files.size(); i++) {
		if (this->files[i]->name == name) {
			return true;
		}
	}
	return false;
}

/*
 * Vrucí true, když složka obsahuje složku se jménem
 */
bool Folder::containFolder(std::string name)
{
	std::unique_lock<std::mutex> lck(foldersMtx);
	// Prochází všechny podsložky a kontroluje podle požadovaného jména
	for (int i = 0; i < this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			return true;
		}
	}
	return false;
}

/*
 * Zamkne zámek proti smazání složky 
 */
bool Folder::lockFolder()
{
	std::unique_lock<std::mutex> lck(lockCounterMtx);
	this->lockCounter++;
	return true;
}

/*
 * Odemkne zámek proti smazání složky 
 */
bool Folder::unLockFolder()
{
	std::unique_lock<std::mutex> lck(lockCounterMtx);
	if (this->lockCounter <= 0) {
		return false;
	}
	this->lockCounter--;
	// Zámek mùže zamknout více procesù
	if (this->lockCounter == 0) {
		return true;
	}
	return true;
}

/*
 * Test, zda je zámek uzamèený 
 */
bool Folder::isLock()
{
	std::unique_lock<std::mutex> lck(lockCounterMtx);
	if (this->lockCounter > 0) {
		return true;
	}
	return false;
}

/*
 * Vrací podsoubor složky podle jména
 */
File* Folder::getFileByName(std::string name)
{
	if (this->containFile(name)) {
		// Prochází všechny podsoubory a kontroluje podle požadovaného jména
		for (int i = 0; i < this->files.size(); i++) {
			if (this->files[i]->name == name) {
				return this->files[i];
			}
		}
	}
	return nullptr;

}

/*
 * Vrací podsložku složky podle jména
 */
Folder* Folder::getFolderByName(std::string name)
{
	if (this->containFolder(name)) {
		// Prochází všechny podsložky a kontroluje podle požadovaného jména
		for (int i = 0; i < this->folders.size(); i++) {
			if (this->folders[i]->name == name) {
				return this->folders[i];

			}
		}
	}
	return nullptr;

}

/* Konstruktor */
Folder::Folder(std::string name, Folder* parent)
{
	this->name = name;
	this->type = FOLDER;
	this->parentFolder = parent;
	this->lockCounter = 0;
}

/* Destruktor */
Folder::~Folder()
{
}
