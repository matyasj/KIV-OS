#include "File.h"
#include "../common/api.h"

/*
 * Vrac� true, kdy� nen� soubor otev�en a otev�e ho .
 */
bool File::setOpened()
{
	std::unique_lock<std::mutex> lck(mtx);
	if (this->isOpened) {
		return false;
	}
	else {
		this->isOpened = true;
		this->inFilePosition = 0;
		return true;
	}
}

/*
 * Vrac� true, kdy� nen� soubor zav�en a zav�e ho .
 */
bool File::setClosed()
{
	std::unique_lock<std::mutex> lck(mtx);
	this->isOpened = false;
	this->inFilePosition = -1;
	return true;
}

/*
 * Zapisuje obsah str do souboru v m�du, kter� ur�uje flag (p�epis, p��pis, p��pis/p�epis od pozice).

 * Vrac� po�et ulo�en�ch znak� 

 * flag = 0 // write bude vkladat od nastavene pozice v souboru        |
 * flag = 1 // write bude prepisovat od nastavene pozice v souboru     |-- Specialni flag ovlivnujici chovani fce write
 * flag = 2 // write bude nastavovat novy obsah souboru na buffer      |
 */
size_t File::write(std::string str, size_t flag)
{
	std::unique_lock<std::mutex> lck(mtx);
	if (this->isOpened) {
		if (flag == 0) {
			this->content = this->content.insert(this->inFilePosition, str);
		}
		else if (flag == 1) {
			this->content = this->content.replace(this->inFilePosition, str.size(), str);
		}
		else if (flag == 2) {
			this->content = std::string(str);
		}

		this->inFilePosition = this->inFilePosition + (int)str.size();
		return str.size();
	}
	else {
		SetLastError(errorFileIsUsed);
		return -1;
	}
}

/*
 * Podobn� jako write ale v�dy v m�du pro p��pis.
 * Vrac� po�et ulo�en�ch znak�
 */
size_t File::append(std::string str)
{
	std::unique_lock<std::mutex> lck(mtx);
	if (this->isOpened) {
		this->content.append(str);
		return str.size();
	}
	else {
		SetLastError(errorFileIsUsed);
		return -1;
	}
}

/*
 * Nastav� pozici v souboru
 */
bool File::setPosition(int newPosition)
{
	std::unique_lock<std::mutex> lck(mtx);
	if (this->content.size() > newPosition && this->isOpened) {
		this->inFilePosition = newPosition;
		return true;
	}
	SetLastError(errorFileIsUsed);
	return false;
}

/*
 * Vrac� obsah souboru
 */
std::string File::read()
{
	std::unique_lock<std::mutex> lck(mtx);
	return this->content + '\0';
}

/*
 * Konstruktor
 */
File::File(std::string name, FileDescriptor* parent, std::string path)
{
	this->name = name;
	this->parrentFolder = parent;
	// Pointer na nov� vytvo�en� string
	this->content = *(new std::string);
	this->inFilePosition = 0;
	this->isOpened = false;
	this->type = FILE;
	this->path = path;
}

/*
 * Destruktor
 */
File::~File()
{
}
