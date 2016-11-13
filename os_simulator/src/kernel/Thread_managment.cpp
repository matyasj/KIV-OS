#include"Thread_managment.h"
#include"../user/Parser/command.h"
#include<thread>

TCB *tcb = new TCB();

/*


*/
std::vector<Thread_ready> thread_ready;
void handleThread(CONTEXT &regs) {
	switch (Get_AL((__int16)regs.Rax)) {
	}
}

void thread(TEntryPoint program, CONTEXT &regs,int id) {
	GetThreadContext(GetCurrentThread(), &regs);
	program(regs);		//todo - nebude cekat
	tcb->execute_thread(id);
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
	int type_command = comm->type_command;
	std::string arguments = "";
	if (comm->has_argument) arguments = comm->arguments.at(0);
	Thread* shell = tcb->get_active_thread_by_type(SHELL);
	int parrent_id = shell->id;
	THandle handleIn = (THandle)regs.Rbx;
	THandle handleOut = (THandle)regs.Rcx;
	std::string current_folder = shell->current_folder;
	int id = tcb->add_thread(type_command, current_folder, RUN,parrent_id,handleIn,handleOut);
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

/*int create_thread(int type_command, std::string current_folder, int parrent_id) {
	return tcb->add_thread(type_command, current_folder, parrent_id);
}*/
int execute_thread(int id) {
	return tcb->execute_thread(id);
}