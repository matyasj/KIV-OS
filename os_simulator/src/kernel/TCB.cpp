#include"TCB.h"
#include<mutex>
#include <sstream>
std::mutex mutex;
TCB::TCB() {
	Thread *thread = new Thread(SHELL, "C/", RUN, -1, nullptr, nullptr);
	threads.push_back(thread);
}

TCB::~TCB() { threads.clear(); }

int TCB::add_thread(int type_command, std::string current_folder, Thread_State state, int parent_id, THandle inputHandle, THandle outputHandle)
{
	Thread *thread = new Thread(type_command, current_folder, state, parent_id,inputHandle,outputHandle);
	std::lock_guard<std::mutex> guard(mutex);
	threads.push_back(thread);
	return thread->id;
}

Thread* TCB::get_thread(int id)
{
	std::lock_guard<std::mutex> guard(mutex);
	for (Thread* t : threads) {
		if (t->id == id) return t;
	}
	SetLastError(threadNotFound);
	return nullptr;
}
Thread* TCB::get_active_thread_by_type(int type_command) {
	std::lock_guard<std::mutex> guard(mutex);
	for (Thread* t : threads) {
		if ((t->type_command == type_command) && t->state == RUN) return t;
	}
	SetLastError(threadNotFound);
	return nullptr;
}

int TCB::execute_thread(int id)
{
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
	for (Thread* t : threads) {
		if (t->id == id) {
			t->current_folder = *folder;
			return 0;
		}
	}
	SetLastError(threadNotFound);
	return threadNotFound;;
}
std::string TCB::print() {
	std::lock_guard<std::mutex> guard(mutex);
	std::stringstream str;
	str << "id \t typ prikazu \t rodic \t stav \t aktualni slozka \t " << std::endl;
	for (Thread* t : threads) {
		str << t->id << "\t" << t->type_command << "\t" << t->parent_id << "\t";
		switch (t->state) {
		case RUN: str << "RUN \t";break;
		case READY: str << "READY \t";break;
		case WAIT: str << "WAIT \t";break;
		case EXECUTED: str << "EXECUTED \t";break;
		case BLOCK: str << "BLOCK \t";break;
		}
		str<< t->current_folder << std::endl;
	}
	return str.str();
}

