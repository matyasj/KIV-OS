#include <iostream>
#include <thread>

#include "shell.h"
#include "Parser/parser.h"
#include "rtl.h"
#include "rtl_error.h"
/*
rdx Command
rbx input Handler
rcx output Handler
rax error
rdi - id thread
*/
size_t __stdcall shell(const CONTEXT &regs) {
	THandle std_in = (THandle)regs.Rbx;
	THandle std_out = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	size_t written;
	size_t pocet = 0;
	int write_flag = (int)regs.Rsi;
	int id = (int)regs.Rdi;
	Parser parser;

	bool run = true;
	while (run) {

		std::string start_text;
		printf_current_folder(id, &start_text);
		start_text += ">";
		Write_File(id, std_out, start_text.c_str(), write_flag, written);
		std::string line = "";
		Read_File(id, std_in, &line, 0, pocet);
		if (pocet > 0) {
			unsigned char last = line.back();
			std::vector<Command> commands;
			if (last != '\0') {
				commands = parser.parse_line(line);
				if (parser.error_class.has_error) {
					Write_File(id, error, parser.error_class.print_last_error().c_str(), 0, written);
					continue;
				}
			}
			else {
				Command com;
				com.name = EXIT_CHAR;
				com.type_command = EXIT;
				commands.push_back(com);
			}
			if (commands.empty()) continue;



			// PROGRAM MANAGER ################################################################################
			// projde vsechny prikazy
			bool end = false;
			for (int i = 0; i < commands.size(); i++) {

				if (commands[i].type_command == EXIT) {
					run = false;
				}

				if (i == commands.size() - 1) {
					end = true;
				}
				else {
					end = false;
				}
				Command com = commands[i];
				bool successs = Start_Program(commands[i], end);
				if (!successs) {
					Write_File(id, error, print_error(Get_Last_Error()).c_str(), 0, written);
				}
			}
			// ##################################################################################################		
		}
	}
	return 0;
}