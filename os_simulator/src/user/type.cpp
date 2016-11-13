#include "type.h"
#include<string>
#include <iostream>


void create(std::string name) {
	THandle fileHelp = Create_File(name.c_str(), FILE_WRITE_ACCESS);
	size_t written;
	std::string str = "prvni soubor";
	Write_File(fileHelp, str.c_str(), 0, written);
	Close_File(fileHelp);
}

/*
Parametry:
1   Zobrazi obsah textoveho souboru
rdx Command
rbx input Handler
rcx output Handler
rax error
rdi  id thread
*/

size_t __stdcall type(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	Command* com = (Command*)regs.Rdx;
	if (com->has_argument) {
		std::string argument = com->arguments.at(0);	// umoznijeme jen 1 argument
		//create(argument);								// TODO - delete - ted pro test
		THandle file = Open_File(argument.c_str(), FILE_READ_ACCESS);
		std::string buffer;
		size_t read;
		if (Get_Last_Error() == 0)
		{
			bool succes = Read_File(file, &buffer, 0, read);
			Close_File(file);
			if (!succes) {
				
			}
			else {
				//const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written
				size_t written;
				//Write_File(output, buffer.c_str(), 0, written);
				std::cout << buffer;
			}
		}
	}
	else {
		//TODO - cist z input
	}
	return 0;
}
