#include "freq.h"
#include <iostream>


/*
Parametry:
0   bude číst z stdin a sestaví frekvenční tabulku bytů, kterou pak vypíše pro všechny byty s frekvencí větší než 0 ve formátu: “0x%hhx : %d”
*/
std::string Freq::run(void) {

	// TESTOVANI BUFFERU

	std::string output = get_whole_input();

	output += "freq";

	save_whole_output(output);

	return "Vytvori frekvencni tabulku bytu ze vstupu...";
}
