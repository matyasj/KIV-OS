#include<string>

#ifndef instruction_H
#define instruction_H

static const int EXIT = 0;
static const int SHELL = 1;
static const int TYPE = 2;
static const int MD = 3;
static const int RD = 4;
static const int ECHO = 5;
static const int WC = 6;
static const int DIR = 7;
static const int RGEN = 8;
static const int RFREQ = 9;
static const int PS = 10;
static const int CD = 11;
static const int SORT = 12;

static const int PIPE = 99;
static const int RED_IN = 98; // <
static const int RED_OUT = 97;//>
static const int RED_IN_ADD = 96;// >>

/* char */
static const std::string  SHELL_CHAR = "cmd";
static const std::string  EXIT_CHAR = "exit";
static const std::string  TYPE_CHAR = "type";
static const std::string  MD_CHAR = "md";
static const std::string  RD_CHAR = "rd";
static const std::string  ECHO_CHAR = "echo";
static const std::string  WC_CHAR = "wc";
static const std::string  DIR_CHAR = "dir";
static const std::string  RGEN_CHAR = "rgen";
static const std::string  RFREQ_CHAR = "freq";
static const std::string  PS_CHAR = "ps";
static const std::string  CD_CHAR = "cd";
static const std::string  SORT_CHAR = "sort";

static const std::string  PIPE_CHAR = "|";
static const std::string  RED_IN_CHAR = "<"; // <
static const std::string  RED_OUT_CHAR = ">";//>
static const std::string  RED_IN_ADD_CHAR = ">>";// >>

#endif // !instruction_H