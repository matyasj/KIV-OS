#include"TCB.h"
TCB::TCB() {}

TCB::~TCB() { threads.clear(); }

THandle TCB::add_thread(int type_command, std::string current_folder, int parrent_id)
{
	Thread *thread = new Thread(type_command, current_folder, RUN, parrent_id);
	threads.push_back(thread);
	return thread;
}

THandle TCB::get_thread(int id)
{
	for (Thread* t : threads) {
		if (t->id == id) return t;
	}
	SetLastError(threadNotFound);
	return nullptr;
}
THandle TCB::get_active_thread_by_type(int type_command) {
	for (Thread* t : threads) {
		if ((t->type_command == type_command) && t->state == RUN) return t;
	}
	SetLastError(threadNotFound);
	return nullptr;
}

int TCB::execute_thread(int id)
{
	int index=-1;
	for (Thread* t : threads) {
		index++;
		if (t->id == id) break;
	}
	if (index != -1) {
		threads.erase(threads.begin() + index);
	}
	else {
		SetLastError(threadNotFound);
		return threadNotFound;
	}
	return 0;
}

int TCB::change_thread_state(int id, Thread_State state)
{
	for (Thread* t : threads) {
		if (t->id == id) {
			t->state = state;
			return 0;
		}
	}
	SetLastError(threadNotFound);
	return threadNotFound;
}

int TCB::change_thread_current_folder(int id, std::string* folder)
{
	for (Thread* t : threads) {
		if (t->id == id) {
			t->current_folder = *folder;
			return 0;
		}
	}
	SetLastError(threadNotFound);
	return threadNotFound;;
}

