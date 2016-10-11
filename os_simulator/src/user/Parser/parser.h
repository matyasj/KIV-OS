#include<string>
#include<vector>
#include"command.h"

#ifndef parser_H
#define parser_H

/*
* rozparsuje zadany string
* @param line co se bude parsovat
* @return vector prikazu, zleva do prava
* prikaz "cd | dir | echo" varti vector v poradi: {cd,dir,echo}
*/
std::vector<Command> parse_line(std::string line);

/*
*Nastartuje parser se ctenim ze stdin
* pravdepodobne jen pro testovaci ucely
*/
void parser_start();
#endif // !parser_h


