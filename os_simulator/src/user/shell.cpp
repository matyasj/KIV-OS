#include <iostream>
#include <thread>

#include "shell.h"
#include "Parser/instruction.h"
#include "Parser/parser.h"
#include "rtl.h"

size_t __stdcall shell(const CONTEXT &regs) {
	/*THandle stdin = Create_File("CONOUT$", FILE_SHARE_WRITE);	//nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	const char* hello = "Hello world from shell!\n";
	size_t written;
	Write_File(stdin, hello, strlen(hello), written);
	Close_File(stdin);*/


	/* TODO - prozatim, jen at se muze testovat */
	int id = (int)regs.Rax;
	Parser parser;
	bool run = 1;
	
	while(run) {

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
		for (int i = 0; i < commands.size(); i++) {
			
			if (commands[i].type_command == EXIT) {
				run = 0;
			}

			Start_Program(commands[i]);
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