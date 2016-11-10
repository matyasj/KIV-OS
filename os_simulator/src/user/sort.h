#pragma once

#include "program.h"


class Sort : public Program {
public:
	void run(void);
private:
	void sort(std::string buf);
	void Sort::sort_vector(std::vector<std::string> lines);
};

