#include "io.h"
#include "kernel.h"
#include "fs.h"
#include "File.h"
#include <iostream>

void HandleIO(CONTEXT &regs) {

	//V ostre verzi pochopitelne do switche dejte volani funkci a ne primo vykonny kod
	/*
		Priklad pouziti metod pro praci se soubory.

		// Vytvori novy soubor a vrati pointer na soubor. Vztvoreny soubor je hned otevreny
		File* newFile = (File*)createFile("C/soubor3.txt", GENERIC_READ);
		
		// Write zapisuje do souboru od pozice v souboru (soubor si udrzuje pozici, od ktere zapisuje)
		int pocetZapsanychZnaku = writeFile(newFile, "Nejaky text");
		
		// Nastavy novou pozici (kurzor) v souboru 0 => zacatek v souboru
		setInFilePosition(newFile, 0);
		
		// Vraci pointer na string. Je to kopie obsahu souboru ne primo obsah aby nebylo mozne soubor prepsat prepsanim stringu.
		std::string fileContent = readFile(newFile);

		// Append zapisuje do souboru vzdy na konec nehlede na pozici v souboru
		int pocetZapsanychZnaku = appendFile(newFile, "\nNejaky dalsi text.");
		std::string fileContent2 = readFile(newFile);

		closeFile(newFile);
		deleteFile(newFile);
	*/


	/*
		Priklad pouziti metod pro praci se slozkami

		createFolder("C/slozka");
		createFolder("C/slozka2");
		createFolder("C/slozka2/dalsiSlozka2");
		createFolder("C/slozka/dalsiSlozka");
		createFolder("C/slozka2/Tmp Slozka");
		printFSTree();
		deleteFolder("C/slozka2/Tmp Slozka");
		printFSTree();

		Priklad vytvoreni slozky a jeji nasledne vytisknuti (pro testovaci ucely)
		Folder* tmp = (Folder*)createFolder("C/slozka");
		recursePrintTree(tmp, "|-");

	*/

	switch (Get_AL((__int16) regs.Rax)) {
		case scCreateFile: {
			// Zatim funguje primo jako shared_read | generic_write
			regs.Rax = (decltype(regs.Rax)) createFile((char*)regs.Rdx, (size_t)regs.Rcx);
			Set_Error(regs.Rax == 0, regs);
		//	printFSTree();
			}
			break;	//scCreateFile


		case scWriteFile: {
			//DWORD written;
			//const bool failed = !WriteFile((HANDLE)regs.Rdx, (void*)regs.Rdi, (DWORD)regs.Rcx, &written, NULL);
			size_t flag = regs.Rcx;
			std::string buffer = (char*)regs.Rdi;
			int written = writeFile((THandle)regs.Rdx, buffer, flag);
			regs.Rax = written;
			Set_Error(written < 0, regs);
		}
			break; //scWriteFile

		case scAppendFile: {
			//DWORD written;
			//const bool failed = !WriteFile((HANDLE)regs.Rdx, (void*)regs.Rdi, (DWORD)regs.Rcx, &written, NULL);
			std::string buffer = (char*)regs.Rdi;
			int written = appendFile((THandle)regs.Rdx, buffer);
			Set_Error(written < 0, regs);
			regs.Rax = written;
		}
			break; //scWriteFile

		case scOpenFile: {
			File* tmpFile = (File*)openFile((char*)regs.Rdx, (size_t)regs.Rcx);
			/*if (tmpFile != nullptr) {
				std::cout << "Soubor: " << tmpFile->name << "\n";
			}*/
			regs.Rax = (decltype(regs.Rax))tmpFile;
			Set_Error(regs.Rax == 0, regs);
			
		}
			break; //scOpenFile
		case scCloseFile: {
			Set_Error(!closeFile((THandle*)regs.Rdx), regs);
		}
			break;	//CloseFile
		case scDeleteFile: {
			Set_Error(!deleteFile((THandle*)regs.Rdx), regs);
		}
			break;	//CloseFile
		case scCreateFolder:{
			std::string name = (char*)regs.Rdx;
			regs.Rax = (decltype(regs.Rax))createFolder(name);
			Set_Error(regs.Rax == false, regs);
	//		printFSTree();
			break;
		}
		case scDeleteFolder: {
			std::string name = (char*)regs.Rdx;
			regs.Rax = (decltype(regs.Rax))deleteFolderByPath(name);
			Set_Error(regs.Rax == false, regs);
//			printFSTree();
		}
			break; //deleteFolder
		case scReadFile: {
			THandle* file = (THandle*)regs.Rdx;
			std::string* buffer;
			buffer = (std::string*)regs.Rdi;
			
			std::string buf = readFile(file);
			*buffer = buf;
			regs.Rax = buffer->size();
			//std::cout << "cteni: " << buf << "\n";
			Set_Error(regs.Rax == -1, regs);
	//		printFSTree();
		}
			break; //readFile

		case scSetInFilePosition: {
			THandle file = (THandle*)regs.Rdx;
			int newPosition = (int)regs.Rcx;
			bool success = setInFilePosition(file, newPosition);
			Set_Error(!success, regs);
		}
			break; //deleteFolder
		/*case scPrintFolder: {
			std::string thread = (char*)regs.Rdx;
			std::string buffer = (char *)regs.Rdi;
			std::string arg = (char *)regs.Rcx;
		}break;*/
		case scGetPipe: {
			THandle* pipeOutput = (THandle*)regs.Rdx;
			THandle* pipeInput = (THandle*)regs.Rcx;
			createPipe(pipeInput, pipeOutput);
		}break;
		case scDir: {
			int id = (int)regs.Rdi;
			char* argument = (char*)regs.Rdx;	//cesta ke slozce -> pripojit k te v TCB
			std::string* buffer = (std::string*)regs.Rcx;
			//todo - vytisknout obsah slozky do bufferu
			*buffer = "DIR READY";
		}
		case scChangeFolder: {
			int id = (int)regs.Rdi;
			char* argument = (char*)regs.Rdx;	//cesta k slozce -> pripojit k te v TCB
			//todo - overit zpravnost a prepnout v tcb
			
			//jak se provede prepnuti v tcb
			//int id_shell = get_active_thread_by_type(SHELL);	//zjistit aktualni aktivni shell
			//int return_value = change_thread_current_folder(id_shell, argument);	// ulozit zmenu
		}
		
	}
}