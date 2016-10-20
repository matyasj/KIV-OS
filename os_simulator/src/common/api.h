#pragma once

#include <Windows.h>

typedef HANDLE THandle;

typedef size_t (__stdcall *TEntryPoint)(const CONTEXT syscall);		//vstupni bod uzivatelskeho programu
typedef void (__stdcall *TSysCall)(CONTEXT &context);			//prototyp funkce, ktera realizuje syscall


/*

   Cisla funkci OS:
	 AH - major cislo aka id skupiny fci
	 AL - minor cislo aka cisle konkretni fce

	 je po ovlani nastavena vlajka carry, tj. CONTEXT::Eflags & CF != 0, pak Rax je kod chyby

      AH == 1 : IO operace
		AL: cislo IO operace	//konzole je take jenom soubor
			1 - otevrit soubor				 IN: rdx je pointer na null-terminated string udavajici file_name; rcx jsou flags k otevreni souboru
											OUT: rax je handle nove otevreneho souboru
			2 - zapis do souboru			 IN: rdx je handle souboru, rdi je pointer na buffer, rcx je pocet bytu v bufferu k zapsani
											OUT: rax je pocet zapsanych bytu
			3 - cti ze souboru
			4 - nastav pozici v souboru
			6 - zavri soubor				 IN: rdx  je handle souboru k zavreni
											OUT: rax != 0 je uspech, jinak chyba

			7 - vytvorit slozku           IN: rdx je pointer na null-terminated string udavajici folder_name i s cestou; Rcx jsou flags
										  OUT: rax == 0 uspech, jinak cislo chyby

		    8 - odstranit slozku	      IN: rdx je pointer na null-terminated string udavajici folder_name i s cestou; Rcx jsou flags
										  OUT: rax == 0 uspech, jinak cislo chyby


												

   Dalsi cisla si doplnte dle potreby


*/


const __int64 CarryFlag = 1;
constexpr DWORD clc(const DWORD flags) {
	return flags & (~CarryFlag);
}

constexpr DWORD stc(const DWORD flags) {
	return flags | CarryFlag;
}

constexpr bool test_cf(const DWORD flags) {
	return flags & CarryFlag;
}

//ah hodnoty 
const __int8 scIO = 1;		//IO operace


//al hodnoty pro scIO 
const __int8 scCreateFile = 1;
const __int8 scWriteFile = 2;
const __int8 scAppendFile = 3;
const __int8 scOpenFile = 4;
const __int8 scReadFile = 5;
const __int8 scCloseFile = 6;
const __int8 scCreateFolder = 7;
const __int8 scDeleteFolder = 8;

// Cisla pro chybyIO
// Soubor nebo slozka nenalezena/neexistuje ... zalezi na tom co se volalo
const __int8 errorFileNotFound = 1;
// Soubor je nastaven jako opened -> proto ho nelze otevrit znovu.
const __int8 errorFileIsUsed = 2;
// Pri vytvareni souboru, ktery jiz existuje
const __int8 errorAlreadyExist = 3;
// Spatna cesta - specialni pripady spatne zadane cesty
const __int8 errorBadPath = 4;
// Dalsi blize nespecifikovana IO chyba :D
const __int8 errorIO = 5;



constexpr __int16 Compose_AX(const __int8 ah, const __int8 al) {
	return (ah << 8) | al;
}

constexpr __int8  Get_AH(const __int16 ax) {
	return ax >> 8;
}

constexpr __int8  Get_AL(const __int16 ax) {
	return ax & 0xFF;
}

