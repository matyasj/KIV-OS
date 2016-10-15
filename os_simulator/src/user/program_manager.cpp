#include <string>

#include "Parser\command.h"
#include "program.h"
#include "dir.h"

std::string execute_command(Command command) {

	std::string output;
	Program *program;


	if (command.name == "dir") {
		program = new Dir();
	}
	else {
		return "Error: Program " + command.name + " doesn't exist!";
	}

	program->init(command.name, NULL, command.arguments);
	output = program->run();

	return output;
}