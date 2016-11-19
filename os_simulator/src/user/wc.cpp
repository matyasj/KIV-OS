#include "wc.h"
#include"Parser\my_string.h"
#include"rtl_error.h"
#include <sstream>
#include <iostream>
/*
Parametry:
1   Vypise informace o souboru (pocet bytu, pocet radek, pocet znaku atd.)
x   Muze fungovat pro vice souboru - pak vypise souhrnou statistiku - mame jeden argument
*/

bool count(std::string text, int& lines, int& bytes, int& words) {
	for (int i = 0;i < text.size();i++) {
		char ch = text.at(i);
		if (ch == '\0') {
			lines++;
			return true;
		}
		bytes++;
		if (ch == '\n') {
			lines++;
		}
		if (isspace(ch))
		{
			if (i != 0 && !isspace(text.at(i - 1)))
				words++;
		}
	}
	return false;
}
std::string print(int& lines, int& bytes, int& words, std::string name) {
	std::stringstream str;
	str << lines << "\t" << words << "\t" << bytes << "\t" << name << std::endl;
	return str.str();
}

size_t __stdcall wc(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	std::string buffer;
	size_t read;
	size_t written;
	int lines = 0, words = 0, bytes = 0;
	if (!arg.empty()) {
		THandle file = Open_File(id,arg.c_str(), FILE_SHARE_READ);
		if (Get_Last_Error() == 0)
		{
			bool succes = Read_File(id,file, &buffer, 0, read);
			Close_File(id,file);
			if (!succes) {
				Write_File(id, error, print_error(Get_Last_Error()).c_str(), 0, written);
			}
			else {
				count(buffer,lines,bytes,words);
				std::string out = print(lines,bytes,words, arg);
				Write_File(id, output, out.c_str(), 0, written);
			}
		}
	}
	else {
		while (true)
		{
			bool succes = Read_File(id, input, &buffer, 0, read);
			bool eof = count(buffer, lines, bytes, words);
			if (eof) break;			
		}
		std::string out = print(lines, bytes, words, "");
		Write_File(id, output, out.c_str(), 0, written);
	}
	return 0;
}
