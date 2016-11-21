#pragma once
#include "File.h"
#include <mutex>
#include <condition_variable> 
#include <queue>
#include <iostream>

/*
 * Objekty t��dy Pipe reprezentuj� synchroniza�n� primitivum roura, kter� slou�� pro meziprocesovou komunikaci, konkr�tn� pro p�ed�v�n� dat mezi dv�ma procesy
 * Instance se chovaj� jako soubor
 * D�d� od t��dy File
 */
class Pipe : public File {
public:
	/* Z�mek, kter� zaji��uje bezpe�nou manipulaci s rourou */
	std::mutex mtx;
	/* Podm�n�n� prom�n�, kter� slou�� pro p�ed�v�n� informac� o velikosti bufferu (jestli je pr�zdn�, nebo pln� - Producent-Konzument) */
	std::condition_variable cv;
	/* Po�et obsazen�ch polo�ek bufferu */
	int buffer_occupancy;
	/* Samotn� buffer (roura), kter� uchov�v� polo�ky roury*/
	std::queue<std::string> my_queue;
	/* Prom�n�, kter� ur�uje, jestli je roura uzav�ena a jestli je z n� mo�n� ��st */
	bool closed_write = false;

	/* Uzav�r� rouru */
	virtual bool setClosed();
	/* Vkl�d� polo�ku do bufferu/roury */
	virtual size_t write(std::string str, size_t flag);
	/* Vyb�r� polo�ku bufferu/roury */
	virtual std::string read();

	/* Konstruktor */
	Pipe::Pipe(std::string name) : File(name, nullptr, "") {
		this->buffer_occupancy = 0;
	}

};