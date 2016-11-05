#include "dir.h"


/*
Parametry:
0   Vypise obsah aktualniho adresare
1   Vypise obsah zadaneho adresare (funguje napr i pro "..")
*/
void Dir::run(void)
{
	save_whole_output("Vypis adresare", true);
	_bout->close();
}
