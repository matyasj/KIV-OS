#pragma once
#include "..\common\api.h"
#include"TCB.h"
#include "kernel.h"
#ifndef Thread_managment_H
#define Thread_managment_H
void handleThread(CONTEXT &regs);
THandle create_thread(int type_command, std::string current_folder, int parrent_id);
int execute_thread(THandle h);
#endif // !Thread_managment_h
