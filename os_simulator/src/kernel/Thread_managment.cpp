#include"Thread_managment.h"
#include"../user/Parser/command.h"
#include<thread>

TCB *tcb = new TCB();

/*


*/
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
p�ed�m ti program a regs ... z regs si vyt�hne� Handlery a nastav� pr�va, z regs rax si z command vyt�hne� typ abys to mohl ulo�it do TCB, 
jinak s regs nic ned�l� a po�le� je programu pomoc� thread(program, regs), ulo�� si do TCB a v�echny ty v�ci kolem

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
	std::thread thread(thread,program,regs,id);
}


/*int create_thread(int type_command, std::string current_folder, int parrent_id) {
	return tcb->add_thread(type_command, current_folder, parrent_id);
}*/
int execute_thread(int id) {
	return tcb->execute_thread(id);
}