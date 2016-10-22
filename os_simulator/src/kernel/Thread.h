#pragma once
#include<string>

#ifndef thread_H
#define thread_H
enum Thread_State{
	RUN,		//bezici
	BLOCK,		//blokovany
	WAIT		//ceka na dokonceni deti
};
class Thread {
public:
	int id;		// id
	int type_command;		// typ prikazu;
	int parent_id = -1;		// id rodice
	Thread_State state;		// stav vlakna
	std::string current_folder;		// aktualni slozka

	Thread(int type_command,std::string current_folder);
	Thread(int type_command, std::string current_folder,Thread_State state);
	Thread(int type_command, std::string current_folder,Thread_State state, int parent_id);
	~Thread();
	

};

#endif