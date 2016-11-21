#include "Pipe.h"

/*
 * Uzavírá rouru
 */
bool Pipe::setClosed()
{
	closed_write = true;
	cv.notify_one();
	return true;
}

/*
 * Vkládá položku do bufferu/roury
 */
size_t Pipe::write(std::string str, size_t flag)
{
	std::unique_lock<std::mutex> lck(mtx);
	// Vložení položky do bufferu
	my_queue.push(std::string(str));
	buffer_occupancy++;
	// Vzbudit jedno vlákno, které spí nad podmínkou prázdného bufferu
	cv.notify_one();
	return str.size();
}

/*
 * Vybírá položku bufferu/roury
 */
std::string Pipe::read()
{
	std::unique_lock<std::mutex> lck(mtx);
	std::string item;
	// Když je buffer prázdný tak zkontrolovat, jestli closed_write flag nastaven na true
	while (buffer_occupancy == 0) {
		// Když ano - ukonèit ètení
		if (closed_write) {
			closed_write = false;
			item = std::string("") + '\0';
			return item;
		}
		//Když ne - zablokovat se a èekat na notify
		cv.wait(lck);
	}
	// Výber položky z bufferu
	item = std::string(my_queue.front());
	my_queue.pop();
	buffer_occupancy--;
	// Test, zda je to posledni výbìr z roury
	if (closed_write && my_queue.empty()) {
		item = std::string(item) + '\0';
	}
	else {
		item = std::string(item);
	}

	return item;
}
