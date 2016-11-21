#include"TCB.h"
#include "fs.h"
#include<mutex>
#include <sstream>
#include <algorithm>
#include <iomanip>

std::mutex mutex;
std::vector<Thread*> threads;

void closeFiles(Thread* t) {
	for (Handle_TCB h : t->handles) {
		if (h.handle != nullptr) {
			closeFile(h.handle);
		}
	}
}

int add_thread(int type_command, std::string name_command ,std::string current_folder, Thread_State state, int parent_id)
{
	Thread* thread = new Thread(type_command,name_command, current_folder, state, parent_id);
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
	auto thread_it = std::find_if(threads.begin(), threads.end(), [type_command](Thread*  f) { return ((f->type_command == type_command) && (f->state == RUN)); });
	if (thread_it != std::end(threads)) {
		return (*thread_it)->id;
	}
	SetLastError(threadNotFound);
	return -1;
}
int execute_thread_tcb_lock_manually(int id){
	int index = -1;
	bool find = false;
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread*  f) { return f->id == id; });
	if (thread_it != std::end(threads)) {
		Thread* t = (*thread_it);
		closeFiles(t);
		threads.erase(thread_it);
		delete(t);
	}
	else {
		SetLastError(threadNotFound);
		return threadNotFound;
	}
	return 0;
}
int execute_thread_tcb(int id)
{
	std::lock_guard<std::mutex> guard(mutex);
	return execute_thread_tcb_lock_manually(id);
}

int change_thread_state_lock_manually(int id, Thread_State state)
{
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread*  f) { return f->id == id; });
	if (thread_it != std::end(threads)) {
		(*thread_it)->state = state;
		return 0;
	}
	SetLastError(threadNotFound);
	return threadNotFound;
}
int change_thread_state(int id, Thread_State state)
{
	std::lock_guard<std::mutex> guard(mutex);
	return change_thread_state_lock_manually(id, state);
	
}
void exit_shell(int id) {
	std::lock_guard<std::mutex> guard(mutex);
	int parrent_id = get_parent_id(id);
	if (parrent_id != -1)
	{
		change_thread_state_lock_manually(parrent_id, RUN);
		execute_thread_tcb_lock_manually(id);
	}
}
int change_thread_current_folder(int id, std::string* folder)
{
	std::lock_guard<std::mutex> guard(mutex);
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread*  f) { return f->id == id; });
	if (thread_it != std::end(threads)) {
		(*thread_it)->current_folder = *folder;
		return 0;
	}
	SetLastError(threadNotFound);
	return threadNotFound;;
}
int get_parent_id(int id) {
	auto thread_it = std::find_if(threads.begin(), threads.end(), [id](Thread*  f) { return f->id == id; });
	if (thread_it != std::end(threads)) return (*thread_it)->parent_id;
	return -1;
}
std::string print_tcb() {
	std::lock_guard<std::mutex> guard(mutex);
	std::stringstream str;
	str << std::setw(6) << "id "<< std::setw(11) << "prikaz "<< std::setw(6) << "rodic " << 
		std::setw(5) << "stav " << std::setw(20) <<"aktualni slozka " << std::endl;
	for (Thread* t : threads) {
		str << std::setw(5) <<  t->id << std::setw(11) << t->name_command << std::setw(6) << t->parent_id << std::setw(5);
		switch (t->state) {
		case INIT:str << "INIT";break;
		case RUN: str << "RUN";break;
		case READY: str << "READY";break;
		case WAIT: str << "WAIT";break;
		case EXECUTED: str << "EXECUTED";break;
		case BLOCK: str << "BLOCK";break;
		}

		str<< std::setw(20) <<t->current_folder << std::endl;
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

