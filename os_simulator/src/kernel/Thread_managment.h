#pragma once
#include "..\common\api.h"
#include"..\common\command.h"
#include"TCB.h"
#include "kernel.h"

#ifndef Thread_managment_H
#define Thread_managment_H
/*
Trida pro ulozeni programu, ktery se spusti
*/
class Thread_ready
{
public:
	/* program*/
	TEntryPoint program;
	/* jeho registry*/
	CONTEXT regs;
	/*id v tcb*/
	int id;
	/*typ instrukce*/
	int type_instruction;
	/* argument
	pokud "", tak neni zadny
	*/
	std::string arg;
	/*konstruktor*/
	Thread_ready();
	/*destruktor*/
	~Thread_ready();
};
/*
vytvori vlakno 
ulozi do tcb se stavem init
@param comm - command
@return id v tcb
*/
int create_thread(Command* comm);
/*
handle thread.
*/
void handleThread(CONTEXT &regs);
/*
Pripravi program ke spusteni
vstup
rdx argument
rbx input Handler
rcx output Handler
rax error
Rdi - id_thread;
Rsi - flag zapisu
*/
void do_thread(TEntryPoint program, CONTEXT &regs);
/*
spusti pripravene programy
*/
void start();
/*
ukonci program
@param id id programu
*/
void execute_thread(int id);
#endif // !Thread_managment_h
