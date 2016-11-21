#include "Folder.h"
#include <iostream>

/*
 * P�id�v� nov� podsoubor
 */
bool Folder::addFile(File* file)
{
	// Kontrola, zda ji� soubor se stejn�m jm�nem slo�ka neobsahuje
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
 * P�id�v� novou podslo�ku
 */
bool Folder::addFolder(Folder* folder)
{
	// Kontrola, zda ji� slo�ku se stejn�m jm�nem slo�ka neobsahuje
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
 * Odebere podsoubor podle jm�na, kdy� nen� otev�en�. Jinak vrac� false
 */
bool Folder::removeFile(std::string name)
{
	// Proch�z� v�echny podsoubory
	for (int i = 0; i < this->files.size(); i++) {
		// Kdy� naraz� na soubor s po�adovan�m jm�nem tak ho sma�e
		if (this->files[i]->name == name) {
			//Kontrola, jestli nen� soubor otev�en
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
 * Odebere podslo�ku podle jm�na. Prov�d� rekurzivn� proch�zen� podslo�ek a postupn� maz�n�.
 * Kdy� naraz� na otev�en� soubor nesma�e pouze cestu k tomuto souboru.
 */
bool Folder::removeFolder(std::string name)
{
	bool result = true;
	for (int i = 0; i != this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			std::vector<Folder*>::iterator it = this->folders[i]->folders.begin();
			int numberOfOpenedFolders = 0;
			while (it != this->folders[i]->folders.end()) {
				// Rekurzivn� maz�n� podslo�ek
				bool success = this->folders[i]->removeFolder((*it)->name);
				if (!success) {
					it++;
					numberOfOpenedFolders++;
					// Kdy� jedna podslo�ka nebude smaz�na z�stane k n� cel� cesta
					result = (result && success);
				}
				else {
					it = this->folders[i]->folders.begin();
					it += numberOfOpenedFolders;
					continue;
				}
			}

			// Mazani soubor�
			std::vector<File*>::iterator fileIt = this->folders[i]->files.begin();
			int numberOfOpenedFiles = 0;
			while (fileIt != this->folders[i]->files.end()) {
				bool success = this->folders[i]->removeFile((*fileIt)->name);
				if (!success) {
					// P�i nesmaz�n� jednoho souboru (je st�le otev�en�) se zachov� cel� cesta slo�ek k n�mu
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
 * Vyp�e obsah slo�ky
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
 * Vruc� true, kdy� slo�ka obsahuje soubor se jm�nem
 */
bool Folder::containFile(std::string name)
{
	// Proch�z� v�echny podsoubory a kontroluje podle po�adovan�ho jm�na
	for (int i = 0; i < this->files.size(); i++) {
		if (this->files[i]->name == name) {
			return true;
		}
	}
	return false;
}

/*
 * Vruc� true, kdy� slo�ka obsahuje slo�ku se jm�nem
 */
bool Folder::containFolder(std::string name)
{
	// Proch�z� v�echny podslo�ky a kontroluje podle po�adovan�ho jm�na
	for (int i = 0; i < this->folders.size(); i++) {
		if (this->folders[i]->name == name) {
			return true;
		}
	}
	return false;
}

/*
 * Vrac� podsoubor slo�ky podle jm�na
 */
File* Folder::getFileByName(std::string name)
{
	if (this->containFile(name)) {
		// Proch�z� v�echny podsoubory a kontroluje podle po�adovan�ho jm�na
		for (int i = 0; i < this->files.size(); i++) {
			if (this->files[i]->name == name) {
				return this->files[i];
			}
		}
	}
	return nullptr;

}

/*
 * Vrac� podslo�ku slo�ky podle jm�na
 */
Folder* Folder::getFolderByName(std::string name)
{
	if (this->containFolder(name)) {
		// Proch�z� v�echny podslo�ky a kontroluje podle po�adovan�ho jm�na
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
