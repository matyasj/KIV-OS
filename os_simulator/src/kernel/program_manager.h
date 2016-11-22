#pragma once
#include "..\common\api.h"
#include "..\common\command.h"
#include "Thread_managment.h"
#include "kernel.h"
#include "fs.h"

#include <string>
#include <vector>

/*
Vytvori program

Rbx - IN THandle / END flag (END flag se zjisti ulozi a pote prepise - dale uz neni treba)
Rcx - OUT THandle

pokud je end flag true, tak vsechny programy spusti a ceka na dokonceni
return
Rax - Error
*/
void handleProgram(CONTEXT &regs);