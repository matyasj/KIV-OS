#pragma once
#include"rtl.h"
#include <string>
#include<vector>

class Program {
protected:
	// jmeno programu
	std::string _name;

	// vstupni data programu (muze byt NULL)
	std::string _input;

	// vstupni parametry programu (muze byt NULL)
	std::vector<std::string> _arguments;
	bool _error=false;

public:
	virtual std::string run(void);
	void Program::init(std::string, std::string, std::vector<std::string>);
	bool Program::hasError();
	Program();
	~Program();	
};

