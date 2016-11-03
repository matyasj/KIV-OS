#pragma once
#include <string>
#include <deque>
#include <mutex>              
#include <condition_variable>


class Buffer {
private:

	// maximalni velikost bufferu
	int const MAX_SIZE = 1000;

	// soucasna velikost bufferu
	int _size;

	// obsah bufferu
	std::deque<std::string> _content;

	// zda je buffer otevren - producent neskoncil cinnost
	bool _open;
	
	// mutex
	std::mutex mtx;

	// cekaci podminka
	std::condition_variable cv;

public:
	Buffer();
	~Buffer();

	// ziskej prvni znak z contentu
	std::string Buffer::pop();

	// uloz znak na posledni misto do contentu
	// vraci 0 pokud vse v poradku
	// vraci 1 pokud je buffer plny
	void Buffer::push(std::string);

	// uzavre pridavani do bufferu - zavola producent na konci cinnosti
	void Buffer::close();

	// zjisti zda je buffer stale otevren - pta se konzument kdyz je buffer prazdny
	bool Buffer::isOpen();

	// zjisti zda je buffer otevreny a je v nem co cist
	bool isReadable();
};
