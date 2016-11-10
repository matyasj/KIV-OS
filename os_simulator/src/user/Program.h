#pragma once
#include"rtl.h"
#include "buffer.h"
#include"rtl_error.h"
#include <string>
#include<vector>

class Program {
protected:
	// jmeno programu
	std::string _name;
	
	// vstupni parametry programu (muze byt NULL)
	std::vector<std::string> _arguments;

	// buffer se vstupnimi daty
	Buffer* _bin;

	// buffer s vystupnimi daty
	Buffer* _bout;

	// error
	bool _error=false;

	std::string _error_string;

	// cte ze vstupniho bufferu tak dlouho, dokud nema vsechny data
	// pro programy, ktere neumoznuji prubezne zpracovani (napr. CD)
	std::string Program::get_whole_input();

	// ulozi cely vystup do vystupniho bufferu - data deli po charu
	// nebo ulozi cely string najednou
	void Program::save_whole_output(std::string, bool);

public:
	virtual void run(void);
	void Program::init(std::string, std::vector<std::string>, Buffer*, Buffer*);
	bool Program::hasError();
	Program();
	~Program();	
};

