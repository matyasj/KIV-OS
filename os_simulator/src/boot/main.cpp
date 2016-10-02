#include <Windows.h>
#include <iostream>

typedef void(__stdcall *TRun_VM)();

int main() {
	SetConsoleTitleA("OS-Simulator");
	HMODULE kernel = LoadLibrary(L"kernel.dll");
	TRun_VM vm = (TRun_VM)GetProcAddress(kernel, "Run_VM");
	if (vm) vm();
	FreeLibrary(kernel);
	
	std::cout << "Hello World!";
	std::cin.get();
	return (EXIT_SUCCESS);
}