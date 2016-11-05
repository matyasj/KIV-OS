#include "cd.h"


/*
Parametry:
0   Zobrazi aktualni adresar a jeho cestu
1   Zmeni pracovni adresar dle zadane cesty

Prepinace
/D  Zmeni aktualni jednotku (to se nas asi netyka)

*/
void Cd::run(void) {

	save_whole_output("Zobrazi aktualni adresar nebo se presune na zadanou cestu", true);
}
