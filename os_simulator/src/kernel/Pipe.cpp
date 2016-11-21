#include "Pipe.h"

bool Pipe::setClosed()
{
	closed_write = true;
	cv.notify_one();
	return true;
}

size_t Pipe::write(std::string str, size_t flag)
{
	std::unique_lock<std::mutex> lck(mtx);
	my_queue.push(std::string(str));
	buffer_occupancy++;
	cv.notify_one();
	return str.size();
}

std::string Pipe::read()
{
	std::unique_lock<std::mutex> lck(mtx);
	std::string item;
	while (buffer_occupancy == 0) {
		if (closed_write) {
			closed_write = false;
			item = std::string("") +'\0';
			return item;
		}
		cv.wait(lck);
	}
	item = std::string(my_queue.front());
	my_queue.pop();
	buffer_occupancy--;
	if (closed_write && my_queue.empty()) {
		item = std::string(item)+'\0';
	}
	else {
		item = std::string(item);
	}
	
	return item;
}
