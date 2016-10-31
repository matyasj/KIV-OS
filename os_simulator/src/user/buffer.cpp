#include "buffer.h"


Buffer::Buffer() {
	
	_content = "";
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
	c = _content[0];
	_content.erase(0, 1);
	_size--;
	cv.notify_one();
			
	return c;
}


void Buffer::push(char c) {

	// pokud je buffer zavreny, neni mozne pridavat
	if (_open) {
		
		// pokud je buffer plny - pozastavit
		std::unique_lock<std::mutex> lck(mtx);
		while (_size == MAX_SIZE) {
			cv.wait(lck);
		}

		// v bufferu je volne misto - uloz znak
		_content += c;
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

bool Buffer::isReadable() {
	
	if(_size == 0 && !_open){
		return false;
	}
	else {
		return true;
	}
}

std::string Buffer::getContent() {

	return _content;
}

void Buffer::setContent(std::string content) {

	_content += content;
	_size = content.size();
	_open = false;
}
