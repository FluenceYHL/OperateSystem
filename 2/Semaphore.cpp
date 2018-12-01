#include "gnuplot.hpp"
#include <condition_variable>
#include <functional>
#include <thread>
#include <queue>
#include <chrono>
#include <map>

class Semaphore {
private:
	std::mutex mtx;
	std::condition_variable cv;
	int count = 10;
public:

	void wait() {
		std::unique_lock<std::mutex> lck(this->mtx);
		--this->count;
		this->cv.wait(lck, [this]{
			return this->count > 0;
		});
	}
	void signal() {
		std::unique_lock<std::mutex> lck(this->mtx);
		++count;
		// if(count <= 0) 
		this->cv.notify_all();
	}
};

int main() {
	std::mutex write_mtx;
	std::unique_lock<std::mutex> lck(write_mtx, std::defer_lock);
	Semaphore sem;
	int count = 0;

	static constexpr int initSize = 30;
	std::thread writer[initSize];
	for(int i = 0;i < initSize; ++i) {
		writer[i] = std::thread([&write_mtx, count]{

			std::unique_lock<std::mutex> lck2(write_mtx);

			std::cout << count << " 个读者\t\t写文件\n";
			std::this_thread::sleep_for(std::chrono::seconds(rand() % 2 + 1));
			// sleep_for
			// wait_for
		});
	}
	std::thread reader[initSize];
	for(int i = 0;i < initSize; ++i) {
		reader[i] = std::thread([&]{

			sem.wait();
			if(++count == 1)
				lck.lock();
			sem.signal();

			std::cout << count << " 个读者\t\t读文件\n";
			std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 2));

			sem.wait();
			if(--count == 0)
				lck.unlock();
			sem.signal();
			
		});
	}
	for(int i = 0;i < initSize; ++i) 
		writer[i].join();
	for(int i = 0; i < initSize; ++i) 
		reader[i].join();
	return 0;
}