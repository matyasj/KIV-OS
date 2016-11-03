#include "freq.h"
#include <iostream>


/*
Parametry:
0   bude číst z stdin a sestaví frekvenční tabulku bytů, kterou pak vypíše pro všechny byty s frekvencí větší než 0 ve formátu: “0x%hhx : %d”
*/
std::string Freq::run(void) {

	while (_bin->isReadable()) {

		std::string pop = _bin->pop();

		int number = std::stoi(pop) + 1;

		_bout->push(std::to_string(number));
	}

	_bout->close();

	return "";
}
