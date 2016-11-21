#include "Folder.h"
#include <iostream>

/*
 * Pøidává nový podsoubor
 */
bool Folder::addFile(File* file)
{
	// Kontrola, zda již soubor se stejným jménem složka neobsahuje
	if (this->containFile(file->name)) {
		std::cout << "File with name " << file->name << "  already exists.\n";
		return false;
	}
	else {
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
		std::cout << "File with name " << folder->name << "  already exists.\n";
		return false;
	}
	else {
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
	// Prochází všechny podsoubory
	for (int i = 0; i < this->files.size(); i++) {
		// Když narazí na soubor s požadovaným jménem tak ho smaže
		if (this->files[i]->name == name) {
			//Kontrola, jestli není soubor otevøen
			if (this->files[i]->isOpened) {
				std::cout << "FILE IS NOT CLOSED. YOU HAVE TO CLOSE FILE BEFORE DELETE!\n";
				return false;
			}
			else {
				this->files.erase(this->files.begin() + i);
				return true;
			}
		}
	}
	std::cout << "FILE NOT FOUND\n";
	return false;
}

/*
 * Odebere podsložku podle jména. Provádí rekurzivní procházení podsložek a postupné mazání.
 * Když narazí na otevøený soubor nesmaže pouze cestu k tomuto souboru.
 */
bool Folder::removeFolder(std::string name)
{
	bool result = true;
	for (int i = 0; i != this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			std::vector<Folder*>::iterator it = this->folders[i]->folders.begin();
			int numberOfOpenedFolders = 0;
			while (it != this->folders[i]->folders.end()) {
				// Rekurzivní mazání podsložek
				bool success = this->folders[i]->removeFolder((*it)->name);
				if (!success) {
					it++;
					numberOfOpenedFolders++;
					// Když jedna podsložka nebude smazána zùstane k ní celá cesta
					result = (result && success);
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
	std::string printSTR = "";
	printSTR += this->name + "\n";
	for (int i = 0; i != this->folders.size(); i++) {
		printSTR += "|- FOLDER::" + this->folders[i]->name + "\n";
	}
	for (int i = 0; i != this->files.size(); i++) {
		printSTR += "|- FILE::" + this->files[i]->name + "\n";
	}
	return printSTR;
}

/*
 * Vrucí true, když složka obsahuje soubor se jménem
 */
bool Folder::containFile(std::string name)
{
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
	// Prochází všechny podsložky a kontroluje podle požadovaného jména
	for (int i = 0; i < this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			return true;
		}
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
}

/* Destruktor */
Folder::~Folder()
{
}
