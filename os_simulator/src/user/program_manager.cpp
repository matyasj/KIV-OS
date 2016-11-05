#include <string>
#include <vector>
#include <iostream>
#include <thread>

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
#include "exit.h"
#include "program_manager.h"


/*
	Spusti serii prikazu
*/
std::string execute_commands(std::vector<Command> commands) {

	std::string output = "";


	// init buffers
	std::vector<Buffer*> buffers;

	std::vector<std::thread> threads;

	for (int i = 0; i <= commands.size(); i++) {

		buffers.push_back(new Buffer());
	}


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
			//input = "FILE INPUT";
		}

		// spusti program ve vlakne
		threads.push_back(std::thread(start_program, commands[i], buffers[i], buffers[i + 1]));
		

		if (commands[i].has_redirect && commands[i].redirect_files.type_redirect == RED_OUT) {
			// TODO uloz input do souboru s prepsanim
			// uloz do souboru commands[i].redirect_files.name
		}

		else if (commands[0].has_redirect && commands[0].redirect_files.type_redirect == RED_OUT_ADD) {
			// TODO pridej input na konec souboru
			// uloz do souboru commands[i].redirect_files.name
		}
	}

	while (buffers[buffers.size() - 1]->isReadable()) {

		std::string pop = buffers[buffers.size() - 1]->pop();

		// jestli ze se jedna o program RGEN, tiskni prubezne
		if (commands[commands.size() - 1].type_command == RGEN) {

			//TODO pomoci shell - print(pop, false); 
			std::cout << pop << " ";
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		else {
			output += pop;
		}		
	}

	
	// program_manager ceka na dokonceni vsech vlaken
	for (int i = 0; i < threads.size(); i++) {
		
		threads[i].join();
	}
	
	return output;
}


/*
	Spusti jeden konktretni program a vrati jeho vystup
*/
void start_program(Command command, Buffer* bin, Buffer* bout)
{
	std::string output = "";
	Program *program = NULL;


	switch (command.type_command) {
	case SHELL:
		// Spusti novy shell
		break;

	case EXIT:
		program = new Exit();
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
		break;
		// TODO - pridat osetreni
	}

	if (program == NULL) {
		// TODO - pridat osetreni
	}
	else {
		program->init(command.name, command.arguments, bin, bout);
		program->run();
	}

	//TODO zrusit poiter
}
