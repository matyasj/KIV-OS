#pragma once
#include<string>
#include<vector>

#ifndef thread_H
#define thread_H
#include "../common/api.h"
enum Thread_State {
	RUN,		//bezici
	READY,		// pripravene
	BLOCK,		//blokovany
	WAIT,		//ceka na dokonceni deti
	EXECUTED	//ukoncene
};
enum Access {
	READ,
	WRITE,
	READ_WRITE,
	APPEND
};
class Handle_TCB {
public:
	THandle handle;
	Access access;
	Handle_TCB(THandle handle,Access access);
	~Handle_TCB();
};
class Thread {
public:
	int id;		// id
	int type_command;		// typ prikazu;
	int parent_id = -1;		// id rodice
	Thread_State state;		// stav vlakna
	std::string current_folder;		// aktualni slozka
	//Handle_TCB inputHandle;		//vstupni handle
	//Handle_TCB outputHandle;	//vystupni handle
	std::vector<Handle_TCB> handles;	//handes

	
	Thread(int type_command, std::string current_folder,Thread_State state, int parent_id,THandle inputHandle,THandle outputHandle);
	~Thread();
	

};

#endif