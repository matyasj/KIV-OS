#include <string>
#include <vector>
#include <iostream>
#include <thread>

#include "program_manager.h"


THandle last_handle;

bool first_program = true;

/*
Spousti jednotlive prikazy a prirazuje jim spravne vstupny a vystupni proudy

Rax - Error
Rbx - IN THandle / END flag (END flag se zjisti ulozi a pote prepise - dale uz neni treba)
Rcx - OUT THandle


*/
void handleProgram(CONTEXT &regs) {

	Command* command = (Command*)regs.Rdx;
	
	int end = (int)regs.Rbx;

	LPCSTR program_name = command->name.c_str();
	TEntryPoint program = (TEntryPoint)GetProcAddress(User_Programs, program_name);

	THandle t;

	// --- INPUT THandle ---
	int id = create_thread(command);
	regs.Rdi = (decltype(regs.Rdi))id;
	regs.Rsi = (decltype(regs.Rsi))2;
	// prvni program v serii
	if (first_program) {

		first_program = false;

		// vstup jde ze souboru
		if (command->has_redirect && command->redirect_files.type_redirect == RED_IN) {

			// ziskej THandle na vstupni soubor a uloz ho do registru
			t = openFile(id,command->redirect_files.name, FILE_SHARE_READ);
			if (t == nullptr) {
				Set_Error(true, regs);
				return;
			}
			regs.Rbx = (decltype(regs.Rbx))t;

		}
		// vstup je prazdny (cte se z konzole)
		else {
			// vstup z konzole
			t = getStdIn();
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
				t = openFile(id, command->redirect_files_out.name, GENERIC_WRITE);
				if (t==nullptr) {
					t = createFile(id, command->redirect_files_out.name, GENERIC_WRITE);
				}
				if (t == nullptr) {
					Set_Error(true, regs);
					return;
				}
				regs.Rcx = (decltype(regs.Rcx))t;

			}
			// vypis na konec souboru
			else if (command->redirect_files_out.type_redirect == RED_OUT_ADD) {

				// ziskej THandle na vystupni soubor a uloz ho do registru
				t = openFile(id, command->redirect_files_out.name, GENERIC_WRITE);
				if (t == nullptr) {
					t = createFile(id, command->redirect_files_out.name, GENERIC_WRITE);
				}
				if (t == nullptr) {
					Set_Error(true, regs);
					return;
				}
				regs.Rsi = (decltype(regs.Rsi))0;
				regs.Rcx = (decltype(regs.Rcx))t;
			}
		}
		// neni vypis do souboru - uloz vystup na konzoli
		else {
			// vypis na konzoli
			t = getStdOut();
			regs.Rcx = (decltype(regs.Rcx))t;
		}
	}
	else {

		// vytvori pipe
		createPipe(&t, &last_handle);
		regs.Rcx = (decltype(regs.Rcx))t;
	}
	//error handle
	regs.Rax = (decltype(regs.Rax))getStdOut();
	// spousteni programu pomoci Thread Management
	do_thread(program, regs);

	// spusteni programu pote co jsou vsechny zpracovany
	if (end) {
		start();
	}
	Set_Error(false, regs);
}