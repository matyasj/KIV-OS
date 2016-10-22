#include "Thread.h"
int id_counter = 1;
Thread::Thread(int type_command, std::string current_folder)
{	
	this->current_folder = current_folder;
	this->type_command = type_command;
	this->id = id_counter;
	id_counter++;
	this->state = RUN;
}

Thread::Thread(int type_command, std::string current_folder, Thread_State state)
{
	this->current_folder = current_folder;
	this->type_command = type_command;
	this->id = id_counter;
	id_counter++;
	this->state = state;
}

Thread::Thread(int type_command, std::string current_folder, Thread_State state, int parent_id)
{
	this->current_folder = current_folder;
	this->type_command = type_command;
	this->id = id_counter;
	id_counter++;
	this->state = state;
	this->parent_id = parent_id;
}

Thread::~Thread()
{
}


