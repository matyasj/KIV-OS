#pragma once
#include "..\common\api.h"
#include"TCB.h"
#include "kernel.h"
#ifndef Thread_managment_H
#define Thread_managment_H
void handleThread(CONTEXT &regs);
void do_thread(TEntryPoint program, CONTEXT &regs);
int execute_thread(int id);
#endif // !Thread_managment_h
