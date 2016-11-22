#ifndef error_H
#define error_H

class Error{
public:
	static const int ERROR_INSTRUCTION = -1;		// chybova instrukce
	static const int UNKNOWN_COMMAND = 1;			//neznama instrukce
	static const int COUNT_OF_ARGUMENT = 2;			//pocet argumentu
	static const int MORE_WORDS_ARGUMENT = 3;		// argument je viceslovny
	static const int MORE_MARKS = 4;				// vice uvozovek
	static const int ONE_MARKS = 5;					// pouze jedna uvozovka
	static const int REDIRECT_ERROR = 6;			// chyba pri presmerovanu
	static const int TOO_MANY_REDIRECT = 7;			// priliz mnoho presmerovani
	static const int EXIT_IN_PIPE = 8;				// spatna syntax exit
	static const int SHELL_IN_PIPE = 9;				//spatna syntax shell
	bool has_error=false;							// byla chyba
	int last_error;									//posledni chyba
	//konstruktor
	Error();
	//destruktor
	~Error();
	//ulozi chybu parseru
	int parser_error(int error);
	//vytiskne ulozenou chybovou hlasku
	std::string print_last_error();
	//vyresetuje aktualni nastaveni
	void reset_parser_error();
	// ma parsesr chybu?
	bool parser_has_error();
};
#endif // !error_H