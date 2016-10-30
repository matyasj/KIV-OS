#include"Thread_managment.h"
TCB *tcb = new TCB();

void handleThread(CONTEXT &regs) {
	switch (Get_AL((__int16)regs.Rax)) {
	case scCreateThread: {
		int type_command = (int)regs.Rdx;
		int parrent_id = (int)regs.Rcx;
		std::string *str = (std::string *)regs.Rbx;
		int i = tcb->add_thread(type_command,*str,parrent_id);
		regs.Rax = (decltype(regs.Rax))i;
	}break;
	case scExecuteThread: {
		int id = (int)regs.Rdx;
		int i = (decltype(regs.Rax)) tcb->execute_thread(id);
	}break;
	
	}
}