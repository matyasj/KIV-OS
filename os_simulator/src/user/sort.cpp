#include "sort.h"
#include"rtl_error.h"
#include"Parser\my_string.h"
#include <iostream>
#include <sstream>

/*
seradi vector stringu a vrati vysledny string
*/
std::string sort_vector(std::vector<std::string> lines) {
	std::stringstream res;
	std::sort(lines.begin(), lines.end());
	for (std::string l : lines) {
		res << l << std::endl;
	}
	return res.str();
}
/*
rozdeli string podle \n
*/
std::vector<std::string> sort(std::string buf) {
	std::stringstream str;
	str << std::endl;
	return split_string(buf, str.str());
}

 size_t __stdcall sort(const CONTEXT &regs) {
	 int id = (int)regs.Rdi;
	 THandle input = (THandle)regs.Rbx;
	 THandle output = (THandle)regs.Rcx;
	 THandle error = (THandle)regs.Rax;
	 std::string arg = (char *)regs.Rdx;
	 std::string buffer;
	 size_t read;
	 size_t written;
	 std::vector<std::string> lines;
	 if (!arg.empty()) {
		 THandle file = Open_File(id,arg.c_str(), FILE_SHARE_READ);
		 if (Get_Last_Error() == 0)
		 {
			 bool succes = Read_File(id,file, &buffer, 0, read);
			 Close_File(id,file);
			 if (!succes) {
				 Write_File(id,error, print_error(Get_Last_Error()).c_str(), 0, written);
			 }
			 else {
				 if (read > 0) buffer.erase(read - 1);
				 lines = sort(buffer);
				 std::string out = sort_vector(lines);
				 Write_File(id,output, out.c_str(), 0, written);
			 }
		 }
		 else {
			 Write_File(id,error, print_error(Get_Last_Error()).c_str(), 0, written);
		 }
	 }
	 else {
		 bool running = true;
		 while (running)
		 {
			 bool succes = Read_File(id,input, &buffer, 0, read);
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
		 Write_File(id,output, out.c_str(), 0, written);
	 }
	 return 0; 
 }
