#pragma once

#include "..\common\api.h"
#include "Parser\command.h"
#include<string>

size_t Get_Last_Error();

THandle Create_File(const char* file_name, size_t flags);
		//podle flags otevre, vytvori soubor a vrati jeho deskriptor
		//vraci nenulovy handle, kdyz vse OK
bool Write_File(const THandle file_handle, const void *buffer, const size_t flag, size_t &written);
		//zapise do souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
		//vraci true, kdyz vse OK
bool Append_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written);
		//zapise na konec souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
		//vraci true, kdyz vse OK
bool Read_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &read);
		//zapise do souboru identifikovaneho deskriptor data z buffer o velikosti buffer_size a vrati pocet zapsanych dat ve writtent
		//vraci true, kdyz vse OK
bool Set_In_File_Position(const THandle file_handle, const size_t new_position);
		//nastavi novou pozici v souboru, od ktere se bude psat do souboru
bool Close_File(const THandle file_handle);
		//uzavre soubor identifikovany pomoci deskriptoru
		//vraci true, kdyz vse OK
bool Delete_File(const THandle file_handle);
		//Smaze soubor identifikovany pomoci deskriptoru
		//vraci true, kdyz vse OK
THandle Open_File(const char* file_name, size_t flags);
//podle flags otevre, vytvori soubor a vrati jeho deskriptor
//vraci nenulovy handle, kdyz vse OK

//vytvori slozku s uvedenou cestou a flags
bool Create_Folder(const std::string file_name, size_t flags);
//odstrani slozku s uvedenou cestou a flags
bool Delete_Folder(const std::string file_name, size_t flags);
bool Print_Folder(THandle handle, std::string arg, const void *buffer);
//vytiskne slozku dle argumentu
// v handle je vychozi vlakno
/*-----------------------------THREAD----------------------------*/

THandle Create_Thread(int type_command,int parrent_id, std::string* path);
//vytvori vlakno a vrati jeho id. Pokud nema rodice, parrent_id = -1;
int Execute_Thread(THandle id);
//ukonci vlakno s  id
THandle search_runing_thread(int type_command);
bool printf_current_folder(THandle h, const void* buffer);

// spusti program s uvedenymi parametry
bool Start_Program(Command command, bool end);