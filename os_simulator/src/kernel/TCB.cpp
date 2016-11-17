#include"TCB.h"
#include "fs.h"
#include<mutex>
#include <sstream>
#include <algorithm>
std::mutex mutex;
std::vector<Thread*> threads;

void closeFiles(Thread* t) {
	for (Handle_TCB h : t->handles) {
		if (h.handle != nullptr) {
			closeFile(h.handle);
		}
	}
}

int add_thread(int type_command, std::string current_folder, Thread_State state, int parent_id, THandle inputHandle, THandle outputHandle)
{
	Thread *thread = new Thread(type_command, current_folder, state, parent_id,inputHandle,outputHandle);
	std::lock_guard<std::mutex> guard(mutex);
	threads.push_back(thread);
	return thread->id;
}

Thread* get_thread(int id)
{
	std::lock_guard<std::mutex> guard(mutex);
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread*  f) { return f->id == id; });
	if (thread_it != std::end(threads)) return (*thread_it);
	SetLastError(threadNotFound);
	return nullptr;
}
int get_active_thread_by_type(int type_command) {
	std::lock_guard<std::mutex> guard(mutex);
	auto thread_it = std::find_if(threads.begin(), threads.end(), [type_command](Thread*  f) { return f->type_command == type_command && f->state == RUN; });
	if (thread_it != std::end(threads)) {
		return (*thread_it)->id;
	}
	SetLastError(threadNotFound);
	return -1;
}

int execute_thread_tcb(int id)
{
	std::lock_guard<std::mutex> guard(mutex);
	int index=-1;
	bool find = false;
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread*  f) { return f->id == id; });
	if (thread_it != std::end(threads)) {
		Thread* t = (*thread_it);
		closeFiles(t);
		threads.erase(thread_it);
	}
	else {
		SetLastError(threadNotFound);
		return threadNotFound;
	}
	return 0;
}

int change_thread_state(int id, Thread_State state)
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

int change_thread_current_folder(int id, std::string* folder)
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

std::string print_tcb() {
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
std::string get_thread_current_folder(int id) {
	std::lock_guard<std::mutex> guard(mutex);
	for (Thread* t : threads) {
		if (t->id == id) {
			return t->current_folder;
		}
	}
	SetLastError(threadNotFound);
	return "";
}
// Prida do TCB tabulky novy filehandler vlaknu s id
bool add_filehandler(int id, THandle file_descriptor, Access access) {
	std::lock_guard<std::mutex> guard(mutex);
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread * f) { return f->id == id; });
	if (thread_it != std::end(threads)) {
		Thread* t = (*thread_it);
		if (t->id == id) {
			t->handles.push_back(Handle_TCB(file_descriptor, access));
			return true;
		}
	}
	return false;
}
// Odebere z TCB tabulky filehandler vlaku s id
bool remove_filehandler(int id, THandle file_descriptor) {
	//TODO: kdyz odstrani, je zavren?
	std::lock_guard<std::mutex> guard(mutex);
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread * f) { return f->id == id; });
	if (thread_it != std::end(threads)) {
		Thread* t = (*thread_it);
		std::vector<Handle_TCB> v = t->handles;
		int index = -1;
		auto it = std::find_if(v.begin(), v.end(), [file_descriptor](Handle_TCB & f) { return f.handle == file_descriptor; });
		if (it != std::end(v)) {
			v.erase(it);
			return true;
		}
		return false;
	}
	return false;
}
// Test, zda vlakno s id obsahuje filedescriptor (true=obsahuje handler)
bool contain_filehandler(int id, THandle file_descriptor) {
	std::lock_guard<std::mutex> guard(mutex);
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread * f) { return f->id == id; });
	if (thread_it != std::end(threads)) {
		Thread* t = (*thread_it);
		std::vector<Handle_TCB> v = t->handles;
		auto it = std::find_if(v.begin(), v.end(), [file_descriptor](Handle_TCB  f) { return f.handle == file_descriptor; });
		if (it != std::end(v)) {
			return true;
		}
	}
	return false;
}

