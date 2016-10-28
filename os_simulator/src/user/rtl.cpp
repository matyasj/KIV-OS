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
	if (failed) LastError = regs.Rax;
		else LastError = 0;

	return !failed;
}



THandle Create_File(const char* file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCreateFile);
	regs.Rdx = (decltype(regs.Rdx)) file_name;
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (THandle) regs.Rax;
}


bool Write_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scWriteFile);
	regs.Rdx = (decltype(regs.Rdx)) file_handle;
	regs.Rdi = (decltype(regs.Rdi)) buffer;
	regs.Rcx = buffer_size;	

	const bool result = Do_SysCall(regs);
	written = regs.Rax;
	return result;
}
bool Read_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &read) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scReadFile);
	regs.Rdx = (decltype(regs.Rdx))file_handle;
	regs.Rdi = (decltype(regs.Rdi))buffer;
	regs.Rcx = buffer_size;
	const bool result = Do_SysCall(regs);
	read = regs.Rax;
	return result;
}
bool Close_File(const THandle file_handle) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCloseFile);
	regs.Rdx = (decltype(regs.Rdx))file_handle;
	return Do_SysCall(regs);
}

THandle Open_File(const char* file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scOpenFile);
	regs.Rdx = (decltype(regs.Rdx))file_name;
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (THandle)regs.Rax;
}
bool Create_Folder(const std::string file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCreateFolder);
	regs.Rdx = (decltype(regs.Rdx))file_name.c_str();
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (bool)regs.Rax;
}
bool Delete_Folder(const std::string file_name, size_t flags) {
	CONTEXT regs = Prepare_SysCall_Context(scIO, scDeleteFolder);
	regs.Rdx = (decltype(regs.Rdx))file_name.c_str();
	regs.Rcx = flags;
	Do_SysCall(regs);
	return (bool)regs.Rax;
}

/*------------------------THREAD-------------------*/
int Create_Thread(int type_command, int parrent_id,std::string* path) {
	CONTEXT regs = Prepare_SysCall_Context(scThread, scCreateThread);
	regs.Rdx = type_command;
	regs.Rcx = parrent_id;
	regs.Rbx = (decltype(regs.Rdx))path;
	Do_SysCall(regs);
	return (int)regs.Rax;
}

int Execute_Thread(int id) {
	CONTEXT regs = Prepare_SysCall_Context(scThread, scExecuteThread);
	regs.Rdx = id;
	Do_SysCall(regs);
	return (int)regs.Rax;
}

