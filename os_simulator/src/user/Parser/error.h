#ifndef error_H
#define error_H

class Error{
public:
	static const int ERROR_INSTRUCTION = -1;		// chybova instrukce
	static const int UNKNOWN_COMMAND = 1;
	static const int COUNT_OF_ARGUMENT = 2;
	static const int MORE_WORDS_ARGUMENT = 3;
	static const int MORE_MARKS = 4;
	static const int ONE_MARKS = 5;
	static const int REDIRECT_ERROR = 6;
	static const int TOO_MANY_REDIRECT = 7;
	static const int EXIT_IN_PIPE = 8;
	static const int SHELL_IN_PIPE = 9;
	bool has_error=false;
	int last_error;
	Error();
	~Error();
	int parser_error(int error);
	std::string print_last_error();
	void reset_parser_error();
	bool parser_has_error();
};
#endif // !error_H