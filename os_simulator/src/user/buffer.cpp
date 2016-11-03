#include "buffer.h"


Buffer::Buffer() {
	
	_size = 0;
	_open = true;
}

Buffer::~Buffer() {}


std::string Buffer::pop() {

	std::string c = "/0";

	// buffer je prazdny a producent jej zavrel
	if (_size == 0 && !_open) {
		
		return c;
	}

	// pokud je buffer prazdny - pozastavit
	std::unique_lock<std::mutex> lck(mtx);
	while (_size == 0) {
		cv.wait(lck);
	}
	
	// v bufferu neco je - vyber 1 znak
	c = _content.front();
	_content.pop_front();
	_size--;
	cv.notify_one();
			
	return c;
}


void Buffer::push(std::string str) {

	// pokud je buffer zavreny, neni mozne pridavat
	if (_open) {
		
		// pokud je buffer plny - pozastavit
		std::unique_lock<std::mutex> lck(mtx);
		while (_size == MAX_SIZE) {
			cv.wait(lck);
		}

		// v bufferu je volne misto - uloz znak
		_content.push_back(str);
		_size++;
		cv.notify_one();
	}
}


void Buffer::close() {

	_open = false;
}


bool Buffer::isOpen() {

	return _open;
}


// TODO - vyresit lepe (EOF)
bool Buffer::isReadable() {
	
	if(_size == 0 && !_open){
		return false;
	}
	else {
		return true;
	}
}