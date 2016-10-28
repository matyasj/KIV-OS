#include <iostream>

#include "shell.h"
#include "Parser/instruction.h"
#include "Parser/parser.h"
#include "program_manager.h"
#include "rtl.h"

size_t __stdcall shell(const CONTEXT &regs) {
	/*THandle stdin = Create_File("CONOUT$", FILE_SHARE_WRITE);	//nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	const char* hello = "Hello world from shell!\n";
	size_t written;
	Write_File(stdin, hello, strlen(hello), written);
	Close_File(stdin);*/


	/* TODO - prozatim, jen at se muze testovat */
	Parser parser;
	bool run = 1;
	
	while(run) {

		// TODO vypise cestu
		std::cout << "Zadej prikaz: ";

		// TODO vyresit pres cteni ze souboru (nebo jak se to bude resit)
		std::string line;
		std::getline(std::cin, line);

		std::vector<Command> commands = parser.parse_line(line);
		line = execute_commands(commands);

		// TODO vyresit pres zapis do souboru
		std::cout << line << std::endl;

		// TODO chtelo by vyresit lepe - prozatim, aby byla kontrola v Shell ne v Parseru
		if ((commands.size() > 0) && (commands[0].type_command == EXIT)) {
			run = 0;
		}			
	}

	return 0;
}