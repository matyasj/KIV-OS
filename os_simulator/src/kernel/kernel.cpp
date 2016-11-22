#pragma once

#include "kernel.h"
#include "io.h"
#include "..\common\command.h"
#include "Thread_managment.h"
#include "program_manager.h"

HMODULE User_Programs;

void Set_Error(const bool failed, CONTEXT &regs) {
	if (failed) {
		regs.EFlags = stc(regs.EFlags);
		int i = GetLastError();
		regs.Rax = GetLastError();
	}
	else {
		regs.EFlags = clc(regs.EFlags);
	}
}


void Initialize_Kernel() {
	User_Programs = LoadLibrary(L"user.dll");	
}

void Shutdown_Kernel() {
	FreeLibrary(User_Programs);
}

void SysCall(CONTEXT &regs) {

	switch (Get_AH((__int16) regs.Rax)) {
		case scIO:		HandleIO(regs);break;		
		case scThread:	handleThread(regs);break;
		case scProgram:	handleProgram(regs); break;
	}

}
/*
Rax - Error
Rbx - IN THandle / END flag(END flag se zjisti ulozi a pote prepise - dale uz neni treba)
Rcx - OUT THandle
Rdx - Command*/
void Run_VM() {
	Initialize_Kernel();

	//spustime shell - v realnem OS bychom ovsem spousteli login
	TEntryPoint shell = (TEntryPoint)GetProcAddress(User_Programs, "shell");
	if (shell) {
		CONTEXT regs;  //ted je regs jenom nejak vyplneno kvuli preakladci
		Command com = Command();
		com.type_command = SHELL;
		com.name = SHELL_CHAR;
		regs.Rbx = true;		// je posledni
		regs.Rdx = (decltype(regs.Rdx))&com;
		handleProgram(regs);
	}
	
	Shutdown_Kernel();
}