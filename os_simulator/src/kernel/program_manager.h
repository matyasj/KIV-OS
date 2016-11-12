#pragma once
#include "..\common\api.h"
#include "..\user\Parser\command.h"
#include "kernel.h"

#include <string>
#include <vector>

//#include "Parser\command.h"

void handleProgram(CONTEXT &regs);
//void execute_commands(std::vector<Command>);