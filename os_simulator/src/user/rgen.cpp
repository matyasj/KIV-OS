#include "rgen.h"
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> run;
/*
zachytava ctrl+z
*/
void catch_ctrl_z(THandle input,int id) {
	std::string buffer;
	size_t read;
	while (true) {
		bool succes = Read_File(id,input, &buffer, 0, read);
		if (read > 0) {
			if (buffer.at(read - 1) == '\0') break;
		}
	}
	run = false;
}
/*
generuje cisla a zapisuje do souboru
*/
void generate(THandle output,int id,int write_flag) {
	int min = 0;
	int max = 9;

	double number = 0.0;
	std::string str = "";
	size_t write;
	while (run) {
		number = ((double)rand() / (double)RAND_MAX);
		str = std::to_string(number) + "\n";
		Write_File(id,output, str.c_str(), write_flag, write);
		write_flag = 0;	// dale se uz jen pripisuje
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
/*
Parametry:
0   bude vypisovat náhodně vygenerovaná čísla v plovoucí čárce na stdout, dokud mu nepřijde znak Ctrl+Z //EOF
*/
size_t __stdcall rgen(const CONTEXT &regs) {
	int id = (int)regs.Rdi;
	THandle input = (THandle)regs.Rbx;
	THandle output = (THandle)regs.Rcx;
	int write_flag = (int)regs.Rsi;
	std::string arg = (char *)regs.Rdx;
	run = true;
	std::thread catch_end(catch_ctrl_z, input,id);
	std::thread gen(generate, output,id,write_flag);
	catch_end.join();
	gen.join();
	return 0;
}