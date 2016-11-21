#include "Pipe.h"

/*
 * Uzav�r� rouru
 */
bool Pipe::setClosed()
{
	closed_write = true;
	cv.notify_one();
	return true;
}

/*
 * Vkl�d� polo�ku do bufferu/roury
 */
size_t Pipe::write(std::string str, size_t flag)
{
	std::unique_lock<std::mutex> lck(mtx);
	// Vlo�en� polo�ky do bufferu
	my_queue.push(std::string(str));
	buffer_occupancy++;
	// Vzbudit jedno vl�kno, kter� sp� nad podm�nkou pr�zdn�ho bufferu
	cv.notify_one();
	return str.size();
}

/*
 * Vyb�r� polo�ku bufferu/roury
 */
std::string Pipe::read()
{
	std::unique_lock<std::mutex> lck(mtx);
	std::string item;
	// Kdy� je buffer pr�zdn� tak zkontrolovat, jestli closed_write flag nastaven na true
	while (buffer_occupancy == 0) {
		// Kdy� ano - ukon�it �ten�
		if (closed_write) {
			closed_write = false;
			item = std::string("") + '\0';
			return item;
		}
		//Kdy� ne - zablokovat se a �ekat na notify
		cv.wait(lck);
	}
	// V�ber polo�ky z bufferu
	item = std::string(my_queue.front());
	my_queue.pop();
	buffer_occupancy--;
	// Test, zda je to posledni v�b�r z roury
	if (closed_write && my_queue.empty()) {
		item = std::string(item) + '\0';
	}
	else {
		item = std::string(item);
	}

	return item;
}
