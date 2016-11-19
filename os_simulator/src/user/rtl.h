#pragma once

#include "..\common\api.h"
#include "..\common\command.h"
#include<string>

size_t Get_Last_Error();

THandle Create_File(int id_thread, const char* file_name, size_t flags);
		//podle flags otevre, vytvori soubor a vrati jeho deskriptor
		//vraci nenulovy handle, kdyz vse OK
bool Write_File(int id_thread, const THandle file_handle, const void *buffer, const size_t flag, size_t &written);
		//zapise do souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
		//vraci true, kdyz vse OK
bool Append_File(int id_thread, const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written);
		//zapise na konec souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
		//vraci true, kdyz vse OK
bool Read_File(int id_thread, const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &read);
		//zapise do souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
		//vraci true, kdyz vse OK
bool Set_In_File_Position(int id_thread, const THandle file_handle, const size_t new_position);
		//nastavi novou pozici v souboru, od ktere se bude psat do souboru
bool Close_File(int id_thread, const THandle file_handle);
		//uzavre soubor identifikovany pomoci deskriptoru
		//vraci true, kdyz vse OK
bool Delete_File(int id_thread, const THandle file_handle);
		//Smaze soubor identifikovany pomoci deskriptoru
		//vraci true, kdyz vse OK
THandle Open_File(int id_thread, const char* file_name, size_t flags);
//podle flags otevre, vytvori soubor a vrati jeho deskriptor
//vraci nenulovy handle, kdyz vse OK

//vytvori slozku s uvedenou cestou a flags
bool Create_Folder(int id_thread, const std::string file_name, size_t flags);
//odstrani slozku s uvedenou cestou a flags
bool Delete_Folder(int id_thread, const std::string file_name, size_t flags);
//change folder - id_thread. a path
bool Change_Folder(int id_thread, const char* path);
//dir - id vlakna a cesta
bool Print_Folder(int id_thread, const char* path, const void* buffer);
//Vytvori Pipe a vrati jeji vstupni a vystupni filedescriptor, ktere pak lze pouzivat jako soubor
bool Create_Pipe(THandle *pipe_output, THandle *pipe_input);

//bool Print_Folder(THandle handle, std::string arg, const void *buffer);
//vytiskne slozku dle argumentu
// v handle je vychozi vlakno
/*-----------------------------THREAD----------------------------*/
void print_ps(const void *buffer);
/*THandle Create_Thread(int type_command,int parrent_id, std::string* path);
//vytvori vlakno a vrati jeho id. Pokud nema rodice, parrent_id = -1;
int Execute_Thread(THandle id);
//ukonci vlakno s  id
THandle search_runing_thread(int type_command);*/
bool printf_current_folder(int id, const void* buffer);

// spusti program s uvedenymi parametry
bool Start_Program(Command command, bool end);