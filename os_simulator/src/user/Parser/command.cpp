#include"instruction.h"
#include"command.h"
#include<iostream>
#include <sstream>


void Command::add_argument(std::string argument) {
	this->arguments.push_back(argument);
	this->has_argument = true;
}
std::string Command::to_string() {
	std::stringstream stream;
	stream << "Name: " << this->name << ", type_command: " << this->type_command << ", arguments: ";
	for (std::string arg : this->arguments) {
		stream << arg+" ";
	}
	if (this->has_redirect) {
		stream << std::endl << "REDIRECT: " << std::endl;
		stream << "name: " << this->redirect_files.name << ", type: " << this->redirect_files.type_redirect;
	}
	stream << std::endl;
	return stream.str();
}
Command::Command(std::string name){
	this->name = name;
}
Command::Command(){
}
Command::~Command()
{
	arguments.clear();
	redirect_files.~Redirect_file();
}
void Command::set_values(std::string name, int type_command) {
	this->type_command = type_command;
	this->name = name;
}
void Command::reset_command() {
	this->has_argument = false;
	this->has_redirect = false;
	this->name="";
	this->arguments.clear();
	this->type_command=-1;
	this->redirect_files.~Redirect_file();
	this->redirect_files;
}


bool Command::add_redirect_file(std::string name, std::string type_redirect){
	int i = -1;
	if (this->has_redirect) {		// pokud uz presmerovani je, neni umozneno dalsi
		return false;	
	}
	if (type_redirect == RED_IN_ADD_CHAR) i = RED_IN_ADD;
	if (type_redirect == RED_IN_CHAR) i = RED_IN;
	if (type_redirect == RED_OUT_CHAR) i = RED_OUT;
	this->has_redirect = true;
	this->redirect_files.name = name;
	this->redirect_files.type_redirect = i;
	return true;
}

Redirect_file::Redirect_file(std::string name, int type_redirect)
{
	this->name = name;
	this->type_redirect = type_redirect;
}

Redirect_file::Redirect_file()
{
}

Redirect_file::~Redirect_file(){
}
