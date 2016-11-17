#pragma once
#include "File.h"
#include <mutex>
#include <condition_variable> 
#include <queue>
#include <iostream>


class Pipe : public File {
public:
	std::mutex mtx;
	std::condition_variable cv;
	int buffer_occupancy;
	std::queue<std::string> my_queue;
	
	virtual bool setClosed();
	virtual size_t write(std::string str, size_t flag);	
	virtual std::string read();

	//Konstruktor
	Pipe::Pipe(std::string name) : File(name, NULL, NULL) {
		this->buffer_occupancy = 0;
	}

};