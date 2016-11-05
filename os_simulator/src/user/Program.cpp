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

		//std::cout << _name << " is consuming " << pop << " ..." << std::endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	
	return content;
}


void Program::save_whole_output(std::string output, bool all_in_one) {

	// ulozi cely string jako jeden prvek
	if (all_in_one) {
		_bout->push(output);
	}
	else {
		// ulozi string po znacich
		char c = '0';
		while (output.size() > 0) {

			c = output[0];
			_bout->push(std::string(1, c));
			output.erase(0, 1);

			//std::cout << _name << " is producing " << output[0] << " ..." << std::endl;
			//std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
	

	_bout->close();
}


void Program::run(void) {

	// Nepouziva se - je implementovana v jednotlivych programech
}
