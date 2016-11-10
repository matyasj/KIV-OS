#include "wc.h"
#include"Parser\my_string.h"
#include <sstream>
#include <iostream>
/*
Parametry:
1   Vypise informace o souboru (pocet bytu, pocet radek, pocet znaku atd.)
x   Muze fungovat pro vice souboru - pak vypise souhrnou statistiku - mame jeden argument
*/
int lines;
int words;
int bytes;
std::string name;
void Wc::run(void) {
	lines = 0, words = 0, bytes = 0;
	name = "";
	if (_arguments.size() != 0) {
		name = _arguments.at(0);
		//pro test
		THandle fileHelp = Create_File(name.c_str(), FILE_WRITE_ACCESS);
		size_t written;
		std::stringstream str;
		str << "ahoj jak se mas a co delas" << std::endl << "ja se mam dobre" << std::endl << "a ja na hovno... nesnasim KIV/OS";
		if (fileHelp == nullptr)
		{
			_error_string = print_error(Get_Last_Error());
			return;
		}
		Write_File(fileHelp, str.str().c_str(), 0, written);
		Close_File(fileHelp);


		// ostre
		THandle file = Open_File(name.c_str(), FILE_READ_ACCESS);
		std::string buffer;
		size_t read;
		if (Get_Last_Error() == 0)
		{
			bool succes = Read_File(file, &buffer, 0, read);
			if (!succes) {
				_error = true;
				_error_string = print_error(Get_Last_Error());
			}
			else {
				char a = buffer.at(buffer.size()-1);
				if (a != '\n') {
					buffer += '\n';
				}
				count(buffer);
				print();
			}
			Close_File(file);
		}
		else {
			_error = true;
			_error_string = print_error(Get_Last_Error());
		}
	}
	else {
		while (true) {
			std::string line;
			std::getline(std::cin, line);
			if (std::cin.eof()) {
				break;
			}
			line += '\n';
			count(line);
			std::cin.clear();
		}
		// TODO = pro PIPE
	/*	while (_bin->isReadable()) {
			std::string pop = _bin->pop();
			count(pop);
		}*/
		print();
	}
}
void Wc::count(std::string text) {
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
	/*text = trim(text);
	int l = std::count(text.begin(), text.end(), '\n')+1; // plus posledni radek - (je trimovano)
	lines +=l;		
	words += std::count(text.begin(), text.end(),' ');		// prvni slovo nema mezeru*/
}
void Wc::print() {
	std::stringstream str;
	str << lines << "\t" << words << "\t" << bytes << "\t" << name; 
	save_whole_output(str.str(), true);
}
