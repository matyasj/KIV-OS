#include"Thread_managment.h"
#include "..\common\command.h"
#include<thread>
#include<iostream>


std::vector<Thread_ready> thread_ready;
void handleThread(CONTEXT &regs) {
	switch (Get_AL((__int16)regs.Rax)) {
	case scPs: {
		std::string* buffer;
		buffer = (std::string*)regs.Rdx;
		std::string buf = print_tcb();
		*buffer = buf;
	}break;
	case scPrintCurrentFolder: {
		std::string* buffer;
		buffer = (std::string*)regs.Rdx;
		int id = (int)regs.Rdi;
		std::string buf = get_thread_current_folder(id);
		*buffer = buf;
	}break;
	}
}
/*void add_first() {
	int id = add_thread(SHELL, "C:\\", RUN, -1, nullptr, nullptr);
}*/
void exit(int id_actual_shell) {
	int parrent_id = get_parent_id(id_actual_shell);
	if (parrent_id != -1)
	{
		change_thread_state(parrent_id, RUN);
		execute_thread(id_actual_shell);
	}
}



void thread(TEntryPoint program, CONTEXT &regs, Thread_ready t) {
	
	int id = t.id;
	std::string str = (t.arg);
	regs.Rdx = (decltype(t.regs.Rdx))(str.c_str());
	//int type_command = com->type_command;
	GetThreadContext(GetCurrentThread(), &regs);
	change_thread_state(id, RUN);
	if (program) {
		if (t.type_instruction == SHELL) {
			int parrent_id = get_parent_id(id);
			change_thread_state(parrent_id, WAIT);
		}
		program(regs);
	}
	else {
		if (t.type_instruction == EXIT) {
			int parrent_id = get_active_thread_by_type(SHELL);
			exit(parrent_id);	
		}
		//std::cout << "Neznam� program";
	}
	execute_thread(id);
}
/*
rax Command, rbx input, rcx output, rdx arguments
p�ed�m ti program a regs ... z regs si vyt�hne� Handlery a nastav� pr�va, z regs rax si z command vyt�hne� typ abys to mohl ulo�it do TCB, 
jinak s regs nic ned�l� a po�le� je programu pomoc� thread(program, regs), ulo�� si do TCB a v�echny ty v�ci kolem

vstup
rdx argument
rbx input Handler
rcx output Handler
rax error

vystup
rbx input Handler
rcx output Handler
rax error
Rdx - arguments
Rdi - id_thread;
*/
void do_thread(TEntryPoint program, CONTEXT &regs) {
	Command* comm = ((Command*)regs.Rdx);
	THandle handleIn = (THandle)regs.Rbx;
	THandle handleOut = (THandle)regs.Rcx;
	int type_command = comm->type_command;
	int parrent_id = get_active_thread_by_type(SHELL);
	std::string current_folder = "C:\\";
	if (parrent_id != -1)
	{
		current_folder = get_thread_current_folder(parrent_id);
	}
	int id = add_thread(type_command,comm->name ,current_folder, READY ,parrent_id,handleIn,handleOut);
	regs.Rdi = id;
	Thread_ready t;
	t.type_instruction = type_command;
	//t.command = *comm;
	std::string arg = "";
	if (comm->has_argument) {
		arg = (comm->arguments.at(0));
		//regs.Rdx = (decltype(regs.Rdx))&arg;
	}
	t.arg = arg;
	t.com = *comm;
	t.id = id;
	t.program = program;
	t.regs = regs;
	thread_ready.push_back(t);
}
void start() {
	std::vector<std::thread> threads;
	for (Thread_ready t : thread_ready) {			
		threads.push_back(std::thread(thread, t.program,t.regs,t));
	}
	thread_ready.clear();
	bool b = thread_ready.empty();
	//wait for them to complete
	for (auto& th : threads)
		th.join();
}

void execute_thread(int id) {
	execute_thread_tcb(id);
}






Thread_ready::Thread_ready()
{
}

Thread_ready::~Thread_ready()
{
}