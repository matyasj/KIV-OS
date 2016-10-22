#include "Program.h"


Program::Program(){}


Program::~Program(){}


void Program::init(std::string name, std::string input, std::vector<std::string> arguments)
{
	_name = name;
	_input = input;
	_arguments = arguments;
}

bool Program::hasError()
{
	return _error;
}


std::string Program::run(void)
{
	// Nepouziva se - je implementovana v jednotlivych programech
	return std::string();
}
