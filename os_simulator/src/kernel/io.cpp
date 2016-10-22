#include "io.h"
#include "kernel.h"
#include "fs.h"
#include "File.h"
#include <iostream>

void HandleIO(CONTEXT &regs) {

	//V ostre verzi pochopitelne do switche dejte volani funkci a ne primo vykonny kod

	switch (Get_AL((__int16) regs.Rax)) {
		case scCreateFile: {
				/*regs.Rax = (decltype(regs.Rax)) CreateFileA((char*)regs.Rdx, GENERIC_READ | GENERIC_WRITE , (DWORD) regs.Rcx, 0, OPEN_EXISTING, 0, 0);
							//zde je treba podle Rxc doresit shared_read, shared_write, OPEN_EXISING, etc. podle potreby
				Set_Error(regs.Rax == 0, regs);	*/
				//File* ahoj = (File*)openFile("C/slozka1/slozka3/slozka4/soubor1.txt", 50);
				/*createFile("C/slozka1/slozka3/slozka4/soubor3.txt", GENERIC_READ);
				createFile("C/slozka1/slozka3/slozka4/soubor4.txt", GENERIC_READ);
				createFile("C/slozka1/slozka3/slozka4/soubor5.txt", GENERIC_READ);
				printFSTree();
				std::cout << readFile(ahoj);
				//deleteFolder("C/slozka1/slozka3/slozka4");
				deleteFile("C/slozka1/slozka3/slozka4/soubor5.txt");
				printFSTree();
				//std::cout << "Jmeno nalezeneho souboru " << ahoj->name << "\n";*/
				THandle file = createFile((char*)regs.Rdx, (DWORD)regs.Rcx);
				if (file==nullptr)
				{
					regs.Rax = errorAlreadyExist;
				}
				regs.Rax = (decltype(regs.Rax))file;
			}
			break;	//scCreateFile


		case scWriteFile: {
				//const bool failed = !WriteFile((HANDLE)regs.Rdx, (void*)regs.Rdi, (DWORD)regs.Rcx, &written, NULL);
				char * buff = (char*)regs.Rdi;
				int write = writeFile((THandle)regs.Rdx,buff);
				regs.Rax = write;
			}
			break; //scWriteFile

		case scOpenFile: {
			THandle ahoj = openFile((char*)regs.Rdx, (DWORD)regs.Rcx);
			if (ahoj == nullptr) {
				regs.Rax = errorFileNotFound;
			}
		}
			break; //scOpenFile
		case scCloseFile: {
			bool failed = closeFile((THandle)regs.Rdx);		
			regs.Rax = failed;
			break;	//CloseFile
		}
		case scReadFile: {
			char* content = readFile((THandle)regs.Rdx);
			regs.Rdi = (decltype(regs.Rdi))content;
			regs.Rax = (decltype(regs.Rax))strlen(content);
		}break;
		case scCreateFolder:{
			char* name = (char*)regs.Rdx;
			bool failed =createFolder(name);
			//printFSTree();
			regs.Rax = failed;
			break;
		}
		case scDeleteFolder: {
			char* name = (char*)regs.Rdx;
			bool failed = deleteFolder(name);
			//printFSTree();
			regs.Rax = failed;
			break;
		}
	}
}