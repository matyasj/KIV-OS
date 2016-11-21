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
			exit_shell(parrent_id);
		}
		//std::cout << "Neznamý program";
	}
	execute_thread(id);
}
int create_thread(Command* comm) {
	int parrent_id = get_active_thread_by_type(SHELL);
	std::string current_folder = "C:\\";
	if (parrent_id != -1)
	{
		current_folder = get_thread_current_folder(parrent_id);
	}
	return add_thread(comm->type_command, comm->name, current_folder, INIT, parrent_id);
}
/*
rax Command, rbx input, rcx output, rdx arguments
pøedám ti program a regs ... z regs si vytáhneš Handlery a nastavíš práva, z regs rax si z command vytáhneš typ abys to mohl uložit do TCB, 
jinak s regs nic nedìláš a pošleš je programu pomocí thread(program, regs), uložíš si do TCB a všechny ty vìci kolem

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
Rsi - flag zapisu
*/
void do_thread(TEntryPoint program, CONTEXT &regs) {
	Command* comm = ((Command*)regs.Rdx);
	THandle handleIn = (THandle)regs.Rbx;
	THandle handleOut = (THandle)regs.Rcx;
	THandle handleError = (THandle)regs.Rax;
	int id = (int)regs.Rdi;
	add_filehandler(id,handleError,WRITE);
	add_filehandler(id, handleIn, READ);
	add_filehandler(id, handleOut, WRITE);
	change_thread_state(id, READY);
	Thread_ready t;
	t.type_instruction = comm->type_command;
	std::string arg = "";
	if (comm->has_argument) {
		arg = (comm->arguments.at(0));
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