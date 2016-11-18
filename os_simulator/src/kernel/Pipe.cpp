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
	while (buffer_occupancy == 0) cv.wait(lck);
	std::string a = std::string(my_queue.front());
	if (closed_write) {
		a = std::string(my_queue.front())+'\0';
	}
	else {
		a = std::string(my_queue.front());
	}
	my_queue.pop();
	buffer_occupancy--;
	return a;
}
