#include "rtl.h"

#include <atomic>

extern "C" __declspec(dllimport) void __stdcall SysCall(CONTEXT &context);


std::atomic<size_t> LastError;

size_t Get_Last_Error() {
	return LastError;
}

CONTEXT Prepare_SysCall_Context(__int8 major, __int8 minor) {
	CONTEXT regs;
	regs.Rax = Compose_AX(major, minor);
	return regs;
}

bool Do_SysCall(CONTEXT &regs) {
	SysCall(regs);

	const bool failed = test_cf(regs.EFlags);
	if (failed) {
		LastError = regs.Rax;
	}
	else {
		LastError = 0;
	}

	return !failed;
}


/*
Pouziti:

THandle* mujPrvniSoubor = Create_File("C/jmeno.txt", FILE_SHARE_READ); // flags -> zatim funguje nativne jako SHARED_READ | GENERIC_WRITE
*/
THandle Create_File(const char* file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCreateFile);
	regs.Rdx = (decltype(regs.Rdx)) file_name;
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (THandle) regs.Rax;
}

/*
Pouziti:

THandle openedFile = Open_File("C/slozka/jmeno.txt", FILE_SHARE_READ);
size_t zapsano = 0; // pocet zapsanych znaku/bytu
Write_File(openedFile, "ahoj", 6, zapsano); 

flag = 0 // write bude vkladat od nastavene pozice v souboru        |
flag = 1 // write bude prepisovat od nastavene pozice v souboru     |-- !!! Specialni flag ovlivnujici chovani fce write
flag = 2 // write bude nastavovat novy obsah souboru na buffer      |

!!! Pri vicenasobnem pouziti funkce Write_File() se zapisuje od konce souboru. Pro zapsani napr. od zacatku se musi nastavit pozice v souboru Set_In_File_Position()

*/
bool Write_File(const THandle file_handle, const void *buffer, const size_t flag, size_t &written) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scWriteFile);
	regs.Rdx = (decltype(regs.Rdx))file_handle;
	regs.Rdi = (decltype(regs.Rdi))buffer;
	regs.Rcx = flag;

	const bool result = Do_SysCall(regs);
	written = regs.Rax;
	return result;
}

/*
Pouziti:

size_t zapsano = 0; // pocet zapsanych znaku/bytu
Append_File(openedFile, "ahoj", 6, zapsano);

Podobny jako write, ale zapisuje vzdy nakonec souboru nehlede na pozici v souboru.
*/
bool Append_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scAppendFile);
	regs.Rdx = (decltype(regs.Rdx))file_handle;
	regs.Rdi = (decltype(regs.Rdi))buffer;
	regs.Rcx = buffer_size;

	const bool result = Do_SysCall(regs);
	written = regs.Rax;
	return result;
}

/*
Pouziti:

std::string a = "";
size_t pocet = 0; // neni potreba pri praci se std::string
Read_File(mujPrvniSoubor, &a, 0, pocet);
*/
bool Read_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &read) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scReadFile);
	regs.Rdx = (decltype(regs.Rdx))file_handle;
	regs.Rdi = (decltype(regs.Rdi))buffer;
	regs.Rcx = buffer_size;
	const bool result = Do_SysCall(regs);
	read = regs.Rax;
	return result;
}

/*
Pouziti:

Set_In_File_Position(openedFile,0); // Nastavi pozici v souboru openedFile na zacatek
*/
bool Set_In_File_Position(const THandle file_handle, const size_t new_position) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scSetInFilePosition);
	regs.Rdx = (decltype(regs.Rdx))file_handle;
	regs.Rcx = new_position;
	const bool result = Do_SysCall(regs);
	
	return result;
}

/*
Pouziti:

THandle openedFile = Open_File("C/slozka/jmenoSouboru.txt", FILE_SHARE_READ);
Close_File(openedFile);
*/
bool Close_File(const THandle file_handle) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCloseFile);
	regs.Rdx = (decltype(regs.Rdx))file_handle;
	return Do_SysCall(regs);
}

/*
Pouziti:

THandle openedFile = Open_File("C/slozka/jmenoSouboru.txt", FILE_SHARE_READ);
*/
THandle Open_File(const char* file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scOpenFile);
	regs.Rdx = (decltype(regs.Rdx))file_name;
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (THandle)regs.Rax;
}
/*
Pouziti:

Create_Folder("C/slozka", 0); // nejjednodussi pripad jak vytvorit slozku
Create_Folder("C/slozka/slozka2", 0);
*/
bool Create_Folder(const std::string file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCreateFolder);
	regs.Rdx = (decltype(regs.Rdx))file_name.c_str();
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (bool)regs.Rax;
}
/*
Pouziti:

Delete_Folder("C/slozka/slozka2",0);
*/
bool Delete_Folder(const std::string file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scDeleteFolder);
	regs.Rdx = (decltype(regs.Rdx))file_name.c_str();
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (bool)regs.Rax;
}
bool Print_Folder(THandle handle, std::string arg, const void *buffer) {
	CONTEXT regs = Prepare_SysCall_Context(scBooth, scPrintFolder);
	regs.Rdx = (decltype(regs.Rdx))handle;
	regs.Rcx = (decltype(regs.Rcx))arg.c_str();
	regs.Rdi = (decltype(regs.Rdi))buffer;
	Do_SysCall(regs);
	return (bool)regs.Rax;
}

/*------------------------THREAD-------------------*/
THandle Create_Thread(int type_command, int parrent_id, std::string* path) {
	CONTEXT regs = Prepare_SysCall_Context(scThread, scCreateThread);
	regs.Rdx = type_command;
	regs.Rcx = parrent_id;
	regs.Rbx = (decltype(regs.Rbx))path;
	Do_SysCall(regs);
	return (THandle)regs.Rax;
}

int Execute_Thread(THandle h) {
	CONTEXT regs = Prepare_SysCall_Context(scThread, scExecuteThread);
	regs.Rdx = (decltype(regs.Rdx))h;
	Do_SysCall(regs);
	return (int)regs.Rax;
}
THandle search_runing_thread(int type_command) {
	CONTEXT regs = Prepare_SysCall_Context(scThread, scSearchThread);
	regs.Rdx = type_command;
	Do_SysCall(regs);
	return (THandle)regs.Rax;
}
bool printf_current_folder(THandle h, const void* buffer) {
	CONTEXT regs = Prepare_SysCall_Context(scThread, scPrintCurrentFolder);
	regs.Rdx = (decltype(regs.Rdx))h;
	regs.Rdi = (decltype(regs.Rdi))buffer;
	Do_SysCall(regs);
	return (bool)regs.Rax;
}



