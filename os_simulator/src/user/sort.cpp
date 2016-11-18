#include "sort.h"
#include"Parser\my_string.h"
#include <iostream>
#include <sstream>


void create_sort(std::string name) {
	//pro test
	THandle fileHelp = Create_File(name.c_str(), FILE_WRITE_ACCESS);
	size_t written;
	std::stringstream str;
	str << "b" << std::endl << "a" << std::endl << "c";
	Write_File(fileHelp, str.str().c_str(), 0, written);
	Close_File(fileHelp);
}
std::string sort_vector(std::vector<std::string> lines) {
	std::stringstream res;
	std::sort(lines.begin(), lines.end());
	for (std::string l : lines) {
		res << l << std::endl;
	}
	return res.str();
}
std::vector<std::string> sort(std::string buf) {
	std::stringstream str;
	str << std::endl;
	return split_string(buf, str.str());
}

 size_t __stdcall sort(const CONTEXT &regs) {
	 int id = (int)regs.Rdi;
	 THandle input = (THandle)regs.Rbx;
	 THandle output = (THandle)regs.Rcx;
	 std::string arg = (char *)regs.Rdx;
	 std::string buffer;
	 size_t read;
	 size_t written;
	 std::vector<std::string> lines;
	 if (!arg.empty()) {
		 //create_sort(argument);								// TODO - delete - ted pro test
		 THandle file = Open_File(arg.c_str(), FILE_READ_ACCESS);
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
				 if (read > 0) buffer.erase(read - 1);
				 lines = sort(buffer);
				 std::string out = sort_vector(lines);
				 size_t written;
				 Write_File(output, out.c_str(), 0, written);
			 }
		 }
	 }
	 else {
		 bool running = true;
		 while (running)
		 {
			 //std::sstream stream
			 bool succes = Read_File(input, &buffer, 0, read);
			 if (read > 0) {
				 if (buffer.at(read - 1) == '\0') {
					 if(read==1)break;
					 buffer.erase(read - 1);
					 running = false;
				 }
			 }
			 std::vector<std::string> pom = sort(buffer);
			 lines.insert(std::end(lines), std::begin(pom), std::end(pom));
		 }
		 std::string out = sort_vector(lines);
		 Write_File(output, out.c_str(), 0, written);
	 }
	 return 0; 
 }
/*
Parametry:
1	Seradi soubory nebo textovy soubor
0   radi ze stdin
*/
/*void Sort::run(void)
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
}*/
