#pragma once

#include <string>
#include "FileDescriptor.h"
#include <mutex>

/*
 * Objekty tøídy File reprezentují soubor ve Filesystemu
 * Dìdí od tøídy FileDescriptor
 */
class File : public FileDescriptor
{
public:
	/* Obsah souboru */
	std::string content;
	/* Flag zda je soubor otevøenı */
	bool isOpened;
	/* Pozice v souboru */
	int inFilePosition;
	/* Pointer na rodièovskou sloku */
	FileDescriptor* parrentFolder;
	/* Úplná cesta k tomuto souboru */
	std::string path;
	/* Zámek, kterı zajišuje bezpeènou manipulaci se souborem */
	std::mutex mtx;

	/* Vrací true, kdy není soubor otevøen a otevøe ho */
	bool setOpened();
	/* Vrací true, kdy není soubor zavøen a zavøe ho */
	virtual bool setClosed();

	/* Zapisuje obsah str do souboru v módu, kterı urèuje flag (pøepis, pøípis, pøípis/pøepis od pozice) */
	virtual size_t write(std::string str, size_t flag);
	/* Podobnì jako write ale vdy v módu pro pøípis */
	size_t append(std::string str);
	/* Nastaví pozici v souboru */
	bool setPosition(int newPosition);
	/* Vrací obsah souboru */
	virtual std::string read();

	//Konstruktor
	File(std::string name, FileDescriptor* parent, std::string path);
	//Destruktor
	~File();
};

