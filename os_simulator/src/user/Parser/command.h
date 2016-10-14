#include<vector>

#ifndef command_H
#define command_H
/*
Trida pro presmerovani souboru
*/
class Redirect_file {
public:
	std::string name;				      //jmeno souboru
	int type_redirect;					  // identifikator type presmerovani -> viz instruction.h
	Redirect_file(std::string name, int type_redirect);
	Redirect_file();
	~Redirect_file();
};
class Command {
public:
	bool has_redirect = false;	          // defaultne nema presmerovani
	bool has_argument = false;			  // defaultne nema argument
	std::string name;				      // name of command - acutally for cout	
	std::vector<std::string> arguments;	  // argumenty
	Redirect_file redirect_files;	      // presmerovani
	int type_command = -1;			      // druh instrukce jako int - viz instruction.h
	Command(std::string name);
	Command();
	~Command();
	/*
	* prida presmerovani
	*@param name jmeno souboru
	*@param type_redirect znak presmerovani
	*/
	bool add_redirect_file(std::string name, std::string type_redirect);
	/*
	*nastavi hodnoty Command
	* @param name jmeno - napr. cd, dir...
	* @param type_command identifikator_instrukce
	*/
	void set_values(std::string name, int type_command);
	//void add_child(Command added, int type_extra_char);
	/*
	* prida argument 
	*/
	void add_argument(std::string argument);
	/* 
	to string
	*/
	std::string to_string();
	/*
	*reset command
	*/
	void reset_command();
};
#endif // !C






