#include "type.h"
#include<string>


/*
Parametry:
1   Zobrazi obsah textoveho souboru
*/
void Type::run(void) {
	// odkomentuj pro otestovani
	std::string name = "";
	if (_arguments.size() != 0) {
		name = _arguments.at(0);
	}
	else {
		_error = true;
		_error_string = print_error(wrongArgument);
		return;
	}
	/*
	THandle fileHelp = Create_File(name.c_str(),FILE_WRITE_ACCESS);
	size_t written;
	std::string str = "prvni soubor";
	if (fileHelp == nullptr)
	{
		save_whole_output("Error", true);
		return;
	}
	Write_File(fileHelp, str.c_str(), 0, written);
	Close_File(fileHelp);*/

	THandle file = Open_File(name.c_str(), FILE_READ_ACCESS);
	std::string buffer;
	size_t read;
	if (Get_Last_Error() ==0)
	{
		bool succes = Read_File(file,&buffer, 0,read);
		if (!succes) {
			_error = true;
			_error_string = print_error(Get_Last_Error());
		}
		else {
			save_whole_output(buffer, false);
		}
		Close_File(file);
	}
	else {
		_error = true;
		_error_string = print_error(Get_Last_Error());
	}

}
