#include "shell.h"
#include"Parser/parser.h"

#include "rtl.h"

size_t __stdcall shell(const CONTEXT &regs) {
	/*THandle stdin = Create_File("CONOUT$", FILE_SHARE_WRITE);	//nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	const char* hello = "Hello world from shell!\n";
	size_t written;
	Write_File(stdin, hello, strlen(hello), written);
	Close_File(stdin);*/

	/* TODO - prozatim, jen at se muze testovat */
	Parser parser;
	parser.parser_start();

	return 0;
}