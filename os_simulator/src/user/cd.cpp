#include "cd.h"
#include"rtl_error.h"



/*
Parametry:
0   Zobrazi aktualni adresar a jeho cestu
1   Zmeni pracovni adresar dle zadane cesty

Prepinace
*/
size_t __stdcall cd(const CONTEXT &regs){
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	int write_flag = (int)regs.Rsi;
	size_t written = 0;
	std::string buffer;
	if (arg.empty()) {
		bool success = printf_current_folder(id, &buffer);
		Write_File(id,output, buffer.c_str(), write_flag, written);
	}
	else {
		bool success = Change_Folder(id, arg.c_str());
		if (!success) {
			int flag = 0;
			size_t flag_t = 0;
			Write_File(id,error, print_error(Get_Last_Error()).c_str(),0 , written);
		}
	}
	return 0;
}
