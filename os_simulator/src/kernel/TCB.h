#pragma once
#include"Thread.h"
#include"../common/api.h"
#include<vector>
#ifndef TCB_H
#define TCB_H


/*
type_command - typ prikazu
current_folder - vychozi adresar
parrent_id - id rodicovskeho thread
return id vytvoreneho vlakna
*/
int add_thread(int type_command, std::string name_command, std::string current_folder, Thread_State state, int parent_id, THandle inputHandle, THandle outputHandle);
/*
id - id hledaneho procesu
pokud nenajde, vrati nullptr
*/
//Thread* get_thread(int id);
/*
najde running thread podle jeho typu
*/
int get_active_thread_by_type(int type_command);
/*
id - id hledaneho procesu
vrati cislo chyby. Vse ok == 0
*/
int execute_thread_tcb(int id);
/*
id - id hledaneho procesu
state - novy stav vlakna
vrati cislo chyby. Vse ok == 0
*/
int change_thread_state(int id, Thread_State state);
/*
id - id hledaneho procesu
folder - nova aktualni slozka
vrati cislo chyby. Vse ok == 0
*/
int change_thread_current_folder(int id, std::string* folder);
std::string print_tcb();
int get_parent_id(int id);


/* TODO: by maty*/
// Ziska string soucasny pracovni adresar procesu/vlakna podle id
std::string get_thread_current_folder(int id);
// Prida do TCB tabulky novy filehandler vlaknu s id
bool add_filehandler(int id, THandle file_descriptor, Access access);
// Odebere z TCB tabulky filehandler vlaku s id
bool remove_filehandler(int id, THandle file_descriptor);
// Test, zda vlakno s id obsahuje filedescriptor (true=obsahuje handler)
bool contain_filehandler(int id, THandle file_descriptor);



#endif
