#pragma once
#include "..\common\api.h"
#include "..\user\Parser\command.h"
#include "..\user\Parser\instruction.h"
#include "Thread_managment.h"
#include "kernel.h"

#include <string>
#include <vector>


void handleProgram(CONTEXT &regs);