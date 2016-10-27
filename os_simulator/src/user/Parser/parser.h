#include<string>
#include<vector>
#include"command.h"
#include"error.h"

#ifndef parser_H
#define parser_H
class Parser {
public:
	Error error_class;
	Parser();
	~Parser();
	/*
	* rozparsuje zadany string
	* @param line co se bude parsovat
	* @return vector prikazu, zleva do prava
	* prikaz "cd | dir | echo" varti vector v poradi: {cd,dir,echo}
	*/
	std::vector<Command> parse_line(std::string line);

private:
	std::string parse_argument(std::string argument);
	Command parse_redirect(std::string str);
	Command parse_instruction_arg(std::string instruction);
};

#endif // !parser_h


