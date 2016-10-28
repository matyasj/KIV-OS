#include"TCB.h"
TCB::TCB() {}

TCB::~TCB() { threads.clear(); }

int TCB::add_thread(int type_command, std::string current_folder, int parrent_id)
{
	Thread *thread = new Thread(type_command, current_folder, RUN, parrent_id);
	threads.push_back(thread);
	return thread->id;
}

Thread* TCB::get_thread(int id)
{
	for (Thread* t : threads) {
		if (t->id == id) return t;
	}
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
	return threadNotFound;;
}

