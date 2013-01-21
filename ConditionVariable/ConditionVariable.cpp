#include <iostream>
#include <condition_variable>
#include <thread>
#include <queue>
#include <string>

std::mutex m;
std::queue<std::string> messages;
std::condition_variable data_condition;

void data_reading()
{
	std::string line;
	while ( getline(std::cin, line) )
	{
		std::lock_guard<std::mutex> lg(m);
		messages.push(line);
		data_condition.notify_one();
	}
}
                                
void data_printing()
{
	while (true)
	{
		std::unique_lock<std::mutex> l(m);
		data_condition.wait( l, [] { return ! messages.empty(); });
		auto message = std::move(messages.front());
		messages.pop();
		l.unlock();
		std::cout << "READ: " << message << std::endl;
	}
}

int main(int argc, char* argv[])
{
	std::thread t_read(data_reading);
	std::thread t_print(data_printing);
	t_read.join();
	t_print.join();
}

