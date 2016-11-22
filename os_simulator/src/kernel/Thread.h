#pragma once
#include<string>
#include<vector>

#ifndef thread_H
#define thread_H
#include "../common/api.h"
/*Stav vlakna*/
enum Thread_State {
	INIT,		// init
	RUN,		//bezici
	READY,		// pripravene
	BLOCK,		//blokovany
	WAIT,		//ceka na dokonceni deti
	EXECUTED	//ukoncene
};
/*pristup*/
enum Access {
	READ,		// read
	WRITE,		// write
	READ_WRITE, //read/write
	APPEND		//append
};
/*
Struktura pro zachovani otevrenych handlu
*/
class Handle_TCB {
public:
	//handle
	THandle handle;
	//pristup
	Access access;
	//konstruktor
	Handle_TCB(THandle handle,Access access);
	//destruktor
	~Handle_TCB();
};
/*
Trida pro uchovani potrebnych udaju v tabulce tcb
*/
class Thread {
public:
	int id;		// id
	int type_command;		// typ prikazu;
	std::string name_command;		// jmeno prikazu
	int parent_id = -1;		// id rodice
	Thread_State state;		// stav vlakna
	std::string current_folder;		// aktualni slozka
	std::vector<Handle_TCB> handles;	//handes

	/*konstruktor*/
	Thread(int type_command, std::string name_command, std::string current_folder,Thread_State state, int parent_id);
	/*destruktor*/
	~Thread();
	

};

#endif