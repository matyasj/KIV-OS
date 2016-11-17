#include"Thread_managment.h"
#include "..\common\command.h"
#include<thread>
#include<iostream>


std::vector<Thread_ready> thread_ready;
void handleThread(CONTEXT &regs) {
	switch (Get_AL((__int16)regs.Rax)) {
	case scPs:
		std::string* buffer;
		buffer = (std::string*)regs.Rdx;
		std::string buf = print_tcb();
		*buffer = buf;
	}
}
void add_first() {
	int id = add_thread(SHELL, "C/", RUN, -1, nullptr, nullptr);
}




void thread(TEntryPoint program, CONTEXT &regs,int id) {
	GetThreadContext(GetCurrentThread(), &regs);
	if (program) {
		program(regs);		//todo - nebude cekat
	}
	else {
		std::cout << "Neznamý program";
	}
	execute_thread(id);
}
/*
rax Command, rbx input, rcx output, rdx arguments
pøedám ti program a regs ... z regs si vytáhneš Handlery a nastavíš práva, z regs rax si z command vytáhneš typ abys to mohl uložit do TCB, 
jinak s regs nic nedìláš a pošleš je programu pomocí thread(program, regs), uložíš si do TCB a všechny ty vìci kolem

rdx Command
rbx input Handler
rcx output Handler
rax error

do rdi prida id thread
*/
void do_thread(TEntryPoint program, CONTEXT &regs) {
	Command* comm = ((Command*)regs.Rdx);
	THandle handleIn = (THandle)regs.Rbx;
	THandle handleOut = (THandle)regs.Rcx;
	int type_command = comm->type_command;
	std::string arguments = "";
	if (comm->has_argument) arguments = comm->arguments.at(0);
	int parrent_id = get_active_thread_by_type(SHELL);
	std::string current_folder = "C:\\";
	if (parrent_id != -1)
	{
		current_folder = get_thread_current_folder(parrent_id);
	}
	int id = add_thread(type_command, current_folder, RUN,parrent_id,handleIn,handleOut);
	regs.Rdi = id;
	Thread_ready t;
	t.id = id;
	t.program = program;
	t.regs = regs;
	thread_ready.push_back(t);
	//std::thread thread(thread,program,regs,id);
	//thread.join();
}
void start() {
	std::vector<std::thread> threads;
	for (Thread_ready t : thread_ready) {
		threads.push_back(std::thread(thread, t.program,t.regs,t.id));
	}
	//wait for them to complete
	for (auto& th : threads)
		th.join();
	thread_ready.clear();
}

void execute_thread(int id) {
	execute_thread_tcb(id);
}