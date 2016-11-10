#include"Thread_managment.h"
TCB *tcb = new TCB();

void handleThread(CONTEXT &regs) {
	switch (Get_AL((__int16)regs.Rax)) {
	case scCreateThread: {
		int type_command = (int)regs.Rdx;
		int parrent_id = (int)regs.Rcx;
		std::string *str = (std::string *)regs.Rbx;
		THandle i = create_thread(type_command, *str, parrent_id);
		regs.Rax= (decltype(regs.Rax))i;
	}break;
	case scExecuteThread: {
		THandle h = (THandle)regs.Rdx;
		int i = (decltype(regs.Rax))execute_thread(h);
	}break;
	case scSearchThread: {
		int type = (int)regs.Rdx;
		THandle t = tcb->get_active_thread_by_type(type);
		Set_Error(t == nullptr, regs);
		regs.Rax = (decltype(regs.Rax))t;
	}break;
	case scPrintCurrentFolder: {
		Thread* h = (Thread*)regs.Rdx;

		std::string* buffer;
		buffer = (std::string*)regs.Rdi;

		std::string buf = h->current_folder;
		*buffer = buf;
		regs.Rax = (decltype(regs.Rax))true;
	}
	}
}

THandle create_thread(int type_command, std::string current_folder, int parrent_id) {
	return tcb->add_thread(type_command, current_folder, parrent_id);
}
int execute_thread(THandle h) {
	Thread * t = (Thread*)h;
	return tcb->execute_thread(t->id);
}