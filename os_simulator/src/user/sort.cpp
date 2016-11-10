#include "sort.h"
#include"Parser\my_string.h"
#include <iostream>
#include <sstream>




void Sort::sort_vector(std::vector<std::string> lines) {
	std::stringstream res;
	std::sort(lines.begin(), lines.end());
	for (std::string l : lines) {
		res << l << std::endl;
	}
	save_whole_output(res.str(), true);
}
void Sort::sort(std::string buf) {
	std::stringstream str;
	str << std::endl;
	sort_vector(split_string(buf,str.str()));
}
/*
Parametry:
1	Seradi soubory nebo textovy soubor
0   radi ze stdin
*/
void Sort::run(void)
{
	std::string name = "";
	if (_arguments.size() != 0) {
		name = _arguments.at(0);
		
		//pro test
		THandle fileHelp = Create_File(name.c_str(), FILE_WRITE_ACCESS);
		size_t written;
		std::stringstream str;
		str << "b" << std::endl << "a" << std::endl << "c";
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
				sort(buffer);
			}
			Close_File(file);
		}
		else {
			_error = true;
			_error_string = print_error(Get_Last_Error());
		}

	}
	else{
		std::vector<std::string> lines;
		while (true) {
			std::string line;
			std::getline(std::cin, line);
			if (std::cin.eof()) {
				break;
			}
			lines.push_back(line);
			std::cin.clear();
		}
		sort_vector(lines);
	}
	//save_whole_output("Seradi prvky", true);
}
