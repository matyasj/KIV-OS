#include "freq.h"
#include <map>
#include <iostream>


/*
Parametry:
0   bude číst z stdin a sestaví frekvenční tabulku bytů, kterou pak vypíše pro všechny byty s frekvencí větší než 0 ve formátu: “0x%hhx : %d”
*/
void Freq::run(void) {

	std::map<char, int> map;
	std::string output = "";

	while (_bin->isReadable()) {

		std::string pop = _bin->pop();

		for (int i = 0; i < pop.size(); i++) {
			
			map[pop[i]]++;
		}		
	}
		
	// TODO - ulozit jako string - nevypisovat
	for (auto const& it : map)
	{
		std::cout << it.first << " : " << it.second << std::endl;
	}


	save_whole_output("", true);
}
