#pragma once

#include <string>
#include "FileDescriptor.h"
#include <mutex>

/*
 * Objekty t��dy File reprezentuj� soubor ve Filesystemu
 * D�d� od t��dy FileDescriptor
 */
class File : public FileDescriptor
{
public:
	/* Obsah souboru */
	std::string content;
	/* Flag zda je soubor otev�en� */
	bool isOpened;
	/* Pozice v souboru */
	int inFilePosition;
	/* Pointer na rodi�ovskou slo�ku */
	FileDescriptor* parrentFolder;
	/* �pln� cesta k tomuto souboru */
	std::string path;
	/* Z�mek, kter� zaji��uje bezpe�nou manipulaci se souborem */
	std::mutex mtx;

	/* Vrac� true, kdy� nen� soubor otev�en a otev�e ho */
	bool setOpened();
	/* Vrac� true, kdy� nen� soubor zav�en a zav�e ho */
	virtual bool setClosed();

	/* Zapisuje obsah str do souboru v m�du, kter� ur�uje flag (p�epis, p��pis, p��pis/p�epis od pozice) */
	virtual size_t write(std::string str, size_t flag);
	/* Podobn� jako write ale v�dy v m�du pro p��pis */
	size_t append(std::string str);
	/* Nastav� pozici v souboru */
	bool setPosition(int newPosition);
	/* Vrac� obsah souboru */
	virtual std::string read();

	//Konstruktor
	File(std::string name, FileDescriptor* parent, std::string path);
	//Destruktor
	~File();
};

