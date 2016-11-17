#include "wc.h"
#include"Parser\my_string.h"
#include <sstream>
#include <iostream>
/*
Parametry:
1   Vypise informace o souboru (pocet bytu, pocet radek, pocet znaku atd.)
x   Muze fungovat pro vice souboru - pak vypise souhrnou statistiku - mame jeden argument
*/


void create_wc(std::string name) {
	THandle fileHelp = Create_File(name.c_str(), FILE_WRITE_ACCESS);
	size_t written;
	std::stringstream str;
	str << "ahoj jak se mas a co delas" << std::endl << "ja se mam dobre" << std::endl << "a ja na hovno... nesnasim KIV/OS"<<std::endl;
	Write_File(fileHelp, str.str().c_str(), 0, written);
	Close_File(fileHelp);
}
void count(std::string text, int& lines, int& bytes, int& words) {
	for (int i = 0;i < text.size();i++) {
		bytes++;
		char ch = text.at(i);
		if (ch == '\n') {
			lines++;
		}
		if (isspace(ch))
		{
			if (i != 0 && !isspace(text.at(i - 1)))
				words++;
		}
	}
}
std::string print(int& lines, int& bytes, int& words, std::string name) {
	std::stringstream str;
	str << lines << "\t" << words << "\t" << bytes << "\t" << name;
	return str.str();
}

size_t __stdcall wc(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	Command* com = (Command*)regs.Rdx;

	int lines = 0, words = 0, bytes = 0;
	if (com->has_argument) {
		std::string argument = com->arguments.at(0);
		create_wc(argument);								// TODO - delete - ted pro test
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
				char a = buffer.at(buffer.size() - 1);
				if (a != '\n') {
					buffer += '\n';
				}
				count(buffer,lines,bytes,words);
				std::string out = print(lines,bytes,words,argument);
				size_t written;
				//std::cout << out;
				Write_File(output, out.c_str(), 0, written);
			}
		}
	}
	else {
		//TODO - read z input
	}

	return 0;
}
