#pragma once
#include "..\common\api.h"
#include"TCB.h"
#include "kernel.h"

#ifndef Thread_managment_H
#define Thread_managment_H
class Thread_ready
{
public:
	TEntryPoint program;
	CONTEXT regs;
	int id;
	int type_instruction;
	std::string arg;

	Thread_ready();
	~Thread_ready();
};


void handleThread(CONTEXT &regs);
void do_thread(TEntryPoint program, CONTEXT &regs);
void start();
void add_first();
void execute_thread(int id);
#endif // !Thread_managment_h
