#pragma once
#include "File.h"
#include <mutex>
#include <condition_variable> 
#include <queue>
#include <iostream>

/*
 * Objekty tøídy Pipe reprezentují synchronizaèní primitivum roura, které slouí pro meziprocesovou komunikaci, konkrétnì pro pøedávání dat mezi dvìma procesy
 * Instance se chovají jako soubor
 * Dìdí od tøídy File
 */
class Pipe : public File {
public:
	/* Zámek, kterı zajišuje bezpeènou manipulaci s rourou */
	std::mutex mtx;
	/* Podmínìná promìná, která slouí pro pøedávání informací o velikosti bufferu (jestli je prázdnı, nebo plnı - Producent-Konzument) */
	std::condition_variable cv;
	/* Poèet obsazenıch poloek bufferu */
	int buffer_occupancy;
	/* Samotnı buffer (roura), kterı uchovává poloky roury*/
	std::queue<std::string> my_queue;
	/* Promìná, která urèuje, jestli je roura uzavøena a jestli je z ní moné èíst */
	bool closed_write = false;

	/* Uzavírá rouru */
	virtual bool setClosed();
	/* Vkládá poloku do bufferu/roury */
	virtual size_t write(std::string str, size_t flag);
	/* Vybírá poloku bufferu/roury */
	virtual std::string read();

	/* Konstruktor */
	Pipe::Pipe(std::string name) : File(name, nullptr, "") {
		this->buffer_occupancy = 0;
	}

};