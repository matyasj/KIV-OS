#include <Windows.h>
#include <iostream>


typedef void(__stdcall *TRun_VM)();

int main() {
	SetConsoleTitleA("OS-Simulator");
	HMODULE kernel = LoadLibrary(L"kernel.dll");
	if (kernel) {
		TRun_VM vm = (TRun_VM)GetProcAddress(kernel, "Run_VM");
		if (vm) vm();
		FreeLibrary(kernel);
	}
	std::cout << "OS terminated!";
	return (EXIT_SUCCESS);
}