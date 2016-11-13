#include <iostream>
#include <thread>

#include "shell.h"
#include "Parser/instruction.h"
#include "Parser/parser.h"
#include "rtl.h"

size_t __stdcall shell(const CONTEXT &regs) {

	/* TODO - prozatim, jen at se muze testovat */
	int id = (int)regs.Rax;
	Parser parser;
	bool run = 1;

	while (run) {

		// TODO vypise cestu
		std::cout << "Zadej prikaz: ";

		// TODO vyresit pres cteni ze souboru (nebo jak se to bude resit)
		std::string line;
		std::cin.clear();
		std::getline(std::cin, line);
		std::vector<Command> commands = parser.parse_line(line);
		if (commands.empty()) continue;



		// PROGRAM MANAGER ################################################################################
		// projde vsechny prikazy
		bool end = false;
		for (int i = 0; i < commands.size(); i++) {

			if (commands[i].type_command == EXIT) {
				run = 0;
			}

			if (i == commands.size() - 1) {
				end = true;
			}
			else {
				end = false;
			}

			Start_Program(commands[i], end);
		}
		// ##################################################################################################		
	}

	return 0;
}


void print(std::string message, bool endl) {

	std::cout << message;

	if (endl) {
		std::cout << std::endl;
	}
}