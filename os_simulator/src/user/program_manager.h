#pragma once

#include <string>
#include <vector>

#include "Parser\command.h"

std::string execute_commands(std::vector<Command>);
std::string start_program(Command command, std::string input);