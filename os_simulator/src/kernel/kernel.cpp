#pragma once

#include "kernel.h"
#include "io.h"
#include"Thread_managment.h"

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
		case scThread: handleThread(regs);break;
		case scBooth: {
			Thread* thread = (Thread*)regs.Rdx; //v Rdx je handle na vlakno
			regs.Rdx = (decltype(regs.Rdx))thread->current_folder.c_str();	// nastavim aktualni slozku
			HandleIO(regs);
	}break;
	}

}

void Run_VM() {
	Initialize_Kernel();

	//spustime shell - v realnem OS bychom ovsem spousteli login
	TEntryPoint shell = (TEntryPoint)GetProcAddress(User_Programs, "shell");
	if (shell) {
		CONTEXT regs;  //ted je regs jenom nejak vyplneno kvuli preakladci
		GetThreadContext(GetCurrentThread(), &regs);  //ale pak bude jeden z registru ukazovat na nejaky startup info blok
		THandle h = create_thread(1, "C/", -1);
		regs.Rax = (decltype(regs.Rax))h;
		shell(regs);
		execute_thread(h);
	}

	Shutdown_Kernel();
}