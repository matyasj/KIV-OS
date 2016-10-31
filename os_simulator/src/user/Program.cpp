#include "program.h"
#include <iostream>

#include <thread>
#include <chrono>


Program::Program(){}


Program::~Program(){}


void Program::init(std::string name, std::vector<std::string> arguments, Buffer* bin, Buffer* bout) {

	_name = name;
	_arguments = arguments;
	_bin = bin;
	_bout = bout;
}


bool Program::hasError() {

	return _error;
}


std::string Program::get_whole_input() {
	
	std::string content = "";

	while (_bin->isReadable()) {

		std::string pop = _bin->pop();

		content += pop;

		std::cout << "Consuming " << pop << " ..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		
	}
	
	return content;
}


void Program::save_whole_output(std::string output) {

	while (output.size() > 0) {
		
		std::cout << "Producing " << output[0] << " ..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));

		_bout->push(output[0]);
		output.erase(0, 1);
	}

	_bout->close();
}


std::string Program::run(void) {

	// Nepouziva se - je implementovana v jednotlivych programech
	return std::string();
}
