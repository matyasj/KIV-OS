#include "rgen.h"
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> run;
void catch_ctrl_z(THandle input) {
	std::string buffer;
	size_t read;
	while (true) {
		bool succes = Read_File(input, &buffer, 0, read);
		if (read > 0) {
			if (buffer.at(read - 1) == '\0') break;
		}
	}
	run = false;
}
void generate(THandle output) {
	int min = 0;
	int max = 9;

	double number = 0.0;
	std::string str = "";
	size_t write;
	while (run) {
		number = ((double)rand() / (double)RAND_MAX);
		str = std::to_string(number) + "\n";
		Write_File(output, str.c_str(), 0, write);
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
	std::string arg = (char *)regs.Rdx;
	run = true;
	std::thread catch_end(catch_ctrl_z, input);
	std::thread gen(generate, output);
	catch_end.join();
	gen.join();
	return 0;
}