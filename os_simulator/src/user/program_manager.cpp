#include <string>
#include <vector>
#include <iostream>

#include "Parser\command.h"
#include "Parser\instruction.h"
#include "program.h"

#include "cd.h"
#include "dir.h"
#include "echo.h"
#include "freq.h"
#include "md.h"
#include "ps.h"
#include "rd.h"
#include "rgen.h"
#include "sort.h"
#include "type.h"
#include "wc.h"
#include "program_manager.h"


/*
	Spusti serii prikazu
	vola se z Parseru
*/
std::string execute_commands(std::vector<Command> commands) {

	std::string input = NULL;
	
	/*
		projde vsechny prikazy - vystup posledniho spusteneho
		je vstup dalsiho spusteneho
		vystup posledniho prikazu je celkovy vystup
	*/
	for (int i = 0; i < commands.size(); i++) {

		// pokud je vstup presmerovan ze souboru
		if (commands[i].has_redirect && commands[i].redirect_files.type_redirect == RED_IN) {

			//TODO precte ze souboru vstup - vrati jako string a ulozi do input
			// nacti soubor commands[i].redirect_files.name
			input = "FILE INPUT";
		}

		// spusti samotny program
		input = start_program(commands[i], input);

		if (commands[i].has_redirect && commands[i].redirect_files.type_redirect == RED_OUT) {
			// TODO uloz input do souboru s prepsanim
			// uloz do souboru commands[i].redirect_files.name
		}

		// TODO - ODKOMENTOVAT AZ BUDE OPRAVENO RED_IN_ADD na RED_OUT_ADD

		//else if (commands[0].has_redirect && commands[0].redirect_files.type_redirect == RED_OUT_ADD) {
			// TODO pridej input na konec souboru
			// uloz do souboru commands[i].redirect_files.name
		//}
	}
	
	// testovaci vypis
	std::cout << input << std::endl;

	return input;
}


/*
	Spusti jeden konktretni program a vrati jeho vystup
*/
std::string start_program(Command command, std::string input)
{
	std::string output = NULL;
	Program *program = NULL;


	switch (command.type_command) {
	case SHELL:
		// Spusti novy shell
		break;

	case EXIT:
		// Ukonci shell
		break;

	case CD:
		program = new Cd();
		break;

	case DIR:
		program = new Dir();
		break;

	case ECHO:
		program = new Echo();
		break;

	case RFREQ:
		program = new Freq();
		break;

	case MD:
		program = new Md();
		break;

	case PS:
		program = new Ps();
		break;

	case RD:
		program = new Rd();
		break;

	case RGEN:
		program = new Rgen();
		break;

	case SORT:
		program = new Sort();
		break;

	case TYPE:
		program = new Type();
		break;

	case WC:
		program = new Wc();
		break;

	default:
		return "Error: Program " + command.name + " doesn't exist!\n";
	}

	if (program == NULL) {
		return "Program lunching failed.\n";
	}
	else {
		program->init(command.name, input, command.arguments);
		output = program->run();
	}

	//TODO zrusit poiter

	return output;
}
