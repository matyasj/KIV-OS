#include<string>
#include<vector>
#include "..\..\common\command.h"
#include"error.h"

#ifndef parser_H
#define parser_H
class Parser {
public:
	Error error_class;			//chyby
	//konstruktor
	Parser();
	//destruktor
	~Parser();
	/*
	* rozparsuje zadany string
	* @param line co se bude parsovat
	* @return vector prikazu, zleva do prava
	* prikaz "cd | dir | echo" varti vector v poradi: {cd,dir,echo}
	*/
	std::vector<Command> parse_line(std::string line);

private:
	/*
	shell a exit bez argumentu, pipe a redirect
	*/
	Command Parser::isSingleCommand(std::string line);
	/*
	* rozparsuje argument
	* muze byt jednoslovny - zatim pouze jeden argument. Vice je error
	* nebo viceslovny - musi zacinat a koncit uvozovkami - jinak error
	*/
	std::string parse_argument(std::string argument);
	/*
	parsuje presmerovani
	*/
	std::vector<Redirect_file>  parse_redirect(std::string str, Command& command);
	/*
	parsuje prikaz 
	nastavi typ instrukce a argument. 
	redukuje <code>instruciton<\code> o rozeznany substring
	*/
	Command parse_instruction_arg(std::string& instruction);
};

#endif // !parser_h


