#include <string>

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


std::string execute_command(Command command) {

	std::string output;
	Program *program;


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
			return "Error: Program " + command.name + " doesn't exist!";
	}

	program->init(command.name, NULL, command.arguments);
	output = program->run();

	return output;
}