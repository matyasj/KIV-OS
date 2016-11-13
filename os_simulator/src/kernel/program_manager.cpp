#include <string>
#include <vector>
#include <iostream>
#include <thread>

#include "program_manager.h"


THandle last_handle;

bool first_program = true;

/*
Rdx - Command
Rbx - END flag / IN THandle (END flag se zjisti ulozi a pote prepise - dale uz neni treba)
Rcx - OUT THandle
*/
void handleProgram(CONTEXT &regs) {

	Command* command = (Command*)regs.Rdx;
	bool end = (bool)regs.Rbx;

	LPCSTR program_name = command->name.c_str();
	TEntryPoint program = (TEntryPoint)GetProcAddress(User_Programs, program_name);

	THandle t;

	// --- INPUT THandle ---

	// prvni program v serii
	if (first_program) {

		first_program = false;

		// vstup jde ze souboru
		if (command->has_redirect && command->redirect_files.type_redirect == RED_IN) {

			// ziskej THandle na vstupni soubor a uloz ho do fronty
			// TODO requestThandle(READ, command->redirect_files.name);
			t = 0;
			regs.Rbx = (decltype(regs.Rbx))t;

			std::cout << "Vstup programu je presmerovan ze souboru " << command->redirect_files.name << std::endl;
		}
		// vstup je prazdny (cte se z konzole ? )
		else {

			// uloz prazdny Handler - neni vstup
			// TODO - vstup z konzole ?
			t = 0;
			regs.Rbx = (decltype(regs.Rbx))t;
		}
	}
	else {
		// vstupem je predchozi THandle
		regs.Rbx = (decltype(regs.Rbx))last_handle;
	}


	// --- OUTPUT THandle ---

	// posledni program v serii	
	if (end) {

		first_program = true;

		// vypis do souboru
		if (command->has_redirect_out) {
			// prepise soubor
			if (command->redirect_files_out.type_redirect == RED_OUT) {

				// ziskej THandle na vystupni soubor a uloz ho do fronty
				// TODO requestThandle(WRITE, command->redirect_files.name);
				t = 0;
				regs.Rbx = (decltype(regs.Rcx))t;

				std::cout << "Vystup programu je presmerovan do souboru " << command->redirect_files_out.name << std::endl;
			}
			// vypis na konec souboru
			else if (command->redirect_files_out.type_redirect == RED_OUT_ADD) {

				// ziskej THandle na vystupni soubor a uloz ho do fronty
				// TODO requestThandle(WRITE, command->redirect_files.name);
				t = 0;
				regs.Rbx = (decltype(regs.Rcx))t;

				std::cout << "Vystup programu je presmerovan na konec souboru " << command->redirect_files_out.name << std::endl;
			}
		}
		// neni vypis do souboru - uloz vystup na konzoli
		else {
			// TODO requestThandle(CONSOLE-WRITE, command->redirect_files.name);
			t = 0;
			regs.Rbx = (decltype(regs.Rcx))t;
		}
	}
	else {

		// vystup je do noveho THandlu
		// TODO requestThandle(WRITE, NEW-PIPE);
		t = 0;
		regs.Rcx = (decltype(regs.Rbx))t;

		// ulozeni handlu pro nasleduji program
		last_handle = t;
	}

	// spousteni programu pomoci Thread Management
	do_thread(program, regs);
	std::cout << "Spousteni programu " << program_name << std::endl;
}