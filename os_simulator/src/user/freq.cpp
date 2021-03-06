#include "freq.h"
#include"rtl_error.h"
#include<sstream>
#include <iomanip>


/*
Parametry:
0   bude číst z stdin a sestaví frekvenční tabulku bytů, kterou pak vypíše pro všechny byty s frekvencí větší než 0 ve formátu: “0x%hhx : %d”
*/
size_t __stdcall freq(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	THandle error = (THandle)regs.Rax;
	std::string arg = (char *)regs.Rdx;
	int write_flag = (int)regs.Rsi;
	std::map<unsigned char, int> map;
	std::string buffer;
	size_t read;
	if (!arg.empty()) {
		Write_File(id,error, print_error(wrongArgument).c_str(), 0, read);
	}else {
		bool run = true;
		while (run) {
			std::string line;
			bool succes = Read_File(id,input, &buffer, 0, read);
			for (int i = 0; i < buffer.size(); i++) {
				if (buffer.at(i) == '\0') {
					run = false;
					break;
				}
				map[buffer.at(i)]++;
			}
		}

		std::stringstream streamO;
		for (auto const& it : map)
		{
			streamO << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << (unsigned int)it.first;
			streamO << " : " << std::dec << it.second << std::endl;
		}
		Write_File(id,output, streamO.str().c_str(), write_flag, read);
	}
	return 0;
}
