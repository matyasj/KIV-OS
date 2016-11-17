#include <iostream>
#include <thread>

#include "shell.h"
#include "Parser/parser.h"
#include "rtl.h"

size_t __stdcall shell(const CONTEXT &regs) {
	/*THandle stdin = Create_File("CONOUT$", FILE_SHARE_WRITE);	//nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	const char* hello = "Hello world from shell!\n";
	size_t written;
	Write_File(stdin, hello, strlen(hello), written);
	Close_File(stdin);*/

	// Standardni vystup - chova se jako soubor -> pri vzpisu na konzoli volat napr: Write_File(std_out, "retezec", strlen("retezec"), written);
	//THandle std_out = Create_File("CONOUT$", FILE_SHARE_WRITE);
	THandle std_in = (THandle)regs.Rbx;
	THandle std_out = (THandle)regs.Rcx;
	Close_File(std_in);
	size_t written;
	// Standardni vstup - chova se jako soubor -> pri vzpisu na konzoli volat napr: Write_File(std_out, "retezec", strlen("retezec"), written);
	//THandle std_in = Create_File("CONIN$", FILE_SHARE_READ);
	size_t pocet = 0;
	
	/* TODO - prozatim, jen at se muze testovat */
	int id = (int)regs.Rax;
	Parser parser;
	bool run = 1;
	
	while (run) {

		// TODO vypise cestu
		//std::cout << "Zadej prikaz: ";
		Write_File(std_out, "Zadej prikaz: ", 1, written);
		std::string line = "";
		Read_File(std_in, &line, 0, pocet);

		// TODO vyresit pres cteni ze souboru (nebo jak se to bude resit)
		//std::string line;
		//std::cin.clear();
		//std::getline(std::cin, line);
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
	Close_File(std_out);

	return 0;
}


void print(std::string message, bool endl) {

	std::cout << message;

	if (endl) {
		std::cout << std::endl;
	}
}