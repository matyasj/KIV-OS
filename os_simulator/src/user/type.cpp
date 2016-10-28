#include "type.h"
#include<string>


/*
Parametry:
1   Zobrazi obsah textoveho souboru
*/
std::string Type::run(void)
{
	std::string name = "";
	if (_arguments.size() != 0) name = _arguments.at(0);
	THandle fileHelp = Create_File(name.c_str(),FILE_WRITE_ACCESS);
	size_t written;
	std::string str = "prvni soubor";
	if (fileHelp == nullptr)
	{
		return "error";
	}
	Write_File(fileHelp, str.c_str(), str.length(), written);
	Close_File(fileHelp);


	
	THandle file = Open_File(name.c_str(), FILE_READ_ACCESS);
	char* buffer = new char[100];
	size_t read;
	if (file!=nullptr)
	{
		bool succes = Read_File(file,buffer, strlen(buffer),read);
		Close_File(file);
		if (!succes) {
			return "Soubor se nepodaøilo precist";
		}
		return read + "\n" + *buffer;
	}
	else {
		Close_File(file);
		return "Soubor neexistuje";
	}

}
