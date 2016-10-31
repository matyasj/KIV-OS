#pragma once

#include <string>
#include <vector>

#include "Parser\command.h"
#include "buffer.h"

std::string execute_commands(std::vector<Command>);
void start_program(Command command, Buffer* bin, Buffer* bout);