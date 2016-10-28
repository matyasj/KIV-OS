#include"parser.h"
#include"my_string.h"
#include"instruction.h"
#include"../program_manager.h"
#include<iostream>
#include <vector>

std::string delimiters[] = { "\""," "};
const int SPACE_INDEX = 1;
const int MARKS_INDEX = 0;
//const int WAS_PARSED = 9;



/*
* rozparsuje argument
* muze byt jednoslovny - zatim pouze jeden argument. Vice je error
* nebo viceslovny - musi zacinat a koncit uvozovkami - jinak error
*/
std::string Parser::parse_argument(std::string argument) {
	if (argument == "") return argument;
	char first = argument.at(0);
	char marks = delimiters[MARKS_INDEX].at(0);
	if (first == marks) {		// obsahuje uvozovky => argument musi byt jimi ohranicen a jedna se o jeden argument
		size_t last_char_index = argument.length() - 1;
		char last = argument.at(last_char_index);// vezmu posledni znak 
		if (last != delimiters[MARKS_INDEX].at(0)) {
			error_class.parser_error(error_class.ONE_MARKS);
			return "";
		}
		std::vector<std::string> data = split_string(argument, delimiters[MARKS_INDEX]); // rozdelit dle uvozovek
		if (data.size() != 3) {  // spravne dvoje uvozovky => velikost 3
			error_class.parser_error(error_class.MORE_MARKS);
			return "";
		}
		return (data.at(1)); // vezmu druhou cast => mezi uvozovkami  
	}
	else { // argument jako jedno slovo. Zatim povolen jen jeden argument
		std::vector<std::string> data = split_string(argument, delimiters[SPACE_INDEX]); // rozdelit dle mezer
		if (data.size() != 1) {
			error_class.parser_error(error_class.MORE_WORDS_ARGUMENT);
			return "";
		}
		else {
			return (data.at(0));
		}
	}
	return 0;
}
/*
rozparsuje presmerovani - ve str je: 
*/
Command Parser::parse_redirect(std::string str) {
	Command command;
	std::vector<std::string> add = split_first_string_more(str, { RED_OUT_ADD_CHAR,RED_IN_CHAR,RED_OUT_CHAR});
	/*prvni je argument*/
	std::string old = add.at(add.size()-1);
	std::string return_value = parse_argument(add.at(0));
	if (error_class.parser_has_error()) {
		error_class.parser_error(error_class.COUNT_OF_ARGUMENT);
		return command;
	}
	command.add_argument(return_value);
	while (old != "") {
		/* dale jen soubory*/
		add = split_first_string_more(add.at(1), { RED_OUT_ADD_CHAR,RED_IN_CHAR,RED_OUT_CHAR });
		return_value = parse_argument(add.at(0));
		if (error_class.parser_has_error()) {
			error_class.parser_error(error_class.REDIRECT_ERROR);
			return command;
		}
		bool was_added = command.add_redirect_file(return_value, old);
		if (!was_added){
			error_class.parser_error(error_class.TOO_MANY_REDIRECT);
			return command;
		}
		old = add.at(add.size() - 1);
	}
	return command;
}

Command Parser::parse_instruction_arg(std::string instruction) {
	size_t pos = 0;
	std::vector<std::string> data = split_first_string(instruction, delimiters[SPACE_INDEX]); // rozdelit podle prvni mezery => prikaz a zbytek
	int command_type = error_class.ERROR_INSTRUCTION;
	const std::string com = data.at(0);
	if (com == EXIT_CHAR) {
		command_type = EXIT;
	}
	if (com == SHELL_CHAR) {
		command_type = SHELL;
	}
	if (com == TYPE_CHAR) {
		command_type = TYPE;
	}
	if (com == MD_CHAR) {
		command_type = MD;
	}
	if (com == ECHO_CHAR) {
		command_type = ECHO;
	}
	if (com == WC_CHAR) {
		command_type = WC;
	}
	if (com == DIR_CHAR) {
		command_type = DIR;
	}
	if (com == RGEN_CHAR) {
		command_type = RGEN;
	}
	if (com == RFREQ_CHAR) {
		command_type = RFREQ;
	}
	if (com == PS_CHAR) {
		command_type = PS;
	}
	if (com == CD_CHAR) {
		command_type = CD;
	}	
	if (com == RD_CHAR) {
		command_type = RD;
	}
	if (com == SORT_CHAR) {
		command_type = SORT;
	}
	if (command_type == error_class.ERROR_INSTRUCTION) {
		error_class.parser_error(error_class.UNKNOWN_COMMAND);
		return Command();
	}
	Command comand;
	if (data.size() == 2) {  // delka 'data' muze byt pouze 2 - instrukce + argument
		comand = parse_redirect(data.at(1));
	}
	comand.set_values(com,command_type);
	return comand;
}

std::vector<Command> Parser::parse_line(std::string line) {
	std::vector<std::string> data = split_string(line, PIPE_CHAR); // rozdelit dle pipe
	bool is_first = true;
	std::vector<Command> commands;
	error_class.reset_parser_error();
	for (std::string token : data) {
		token = trim(token);
		if (token.length() == 0) {
			continue; // pokud je prazdny token, pokracuje
		}
		Command com = parse_instruction_arg(token);
		commands.push_back(com);
	}
	if (error_class.parser_has_error()) {
		commands.clear();
		return commands;
	}
	return commands;
}

Parser::Parser()
{

	
}

Parser::~Parser()
{

	this->error_class.~Error();
}
