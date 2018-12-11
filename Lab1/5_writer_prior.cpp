/*********************************************************************************
  *Copyright(C),Your Company
  *FileName:     Semaphore.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:         2018.11.29
  *Description:  使用 C++11 解决生产者-消费者问题
  *Compile:      g++ -std=c++14 Semaphore.cpp -o pc -lpthread
  *Function List:  
  *History:  
     1.Date:         2018.11.29
       Author:       刘畅
       Modification: 完成了基本功能
       Problem:      最后将 count 改为 std::atomic 原子变量
     1.Date:         2018.12.11
       Author:       刘畅
       Modification: 改成了“读写公平”
       Problem:      
**********************************************************************************/
#include "gnuplot.hpp"
#include <condition_variable>
#include <functional>
#include <thread>
#include <queue>
#include <chrono>
#include <atomic>
#include <map>

class Semaphore {  // C++11 模拟信号量
public:
	explicit Semaphore(const int _count) : count(_count) {}
	void wait() { 
		std::unique_lock<std::mutex> lck(this->mtx);
		if(--this->count < 0)
			this->cv.wait(lck);
	}
	void signal() {
		std::unique_lock<std::mutex> lck(this->mtx);
		if(++count <= 0) 
			this->cv.notify_all();  // 通知操作
	}
private:
	std::mutex mtx;
	std::condition_variable cv;
	int count = 10; // 信号变量维护的访问个数
};

int main() {
	Semaphore fmutex(1), readMutex(1), writeMutex(1), queue(1);
	int read_count = 0;
	int write_count = 0;
	std::thread reader([&]{
		while(true) {
			queue.wait();
			readMutex.wait();
			if(read_count == 0)
				fmutex.wait();
			++read_count;
			readMutex.signal();
			queue.signal();

			std::cout << "读.....\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));

			readMutex.wait();
			if(--read_count == 0)
				fmutex.signal();
			readMutex.signal();
		}
	});
	std::thread writer([&]{
		while(true) {
			writeMutex.wait();
			if(write_count == 0)
				queue.wait();
			++write_count;
			writeMutex.signal();

			fmutex.wait();
			std::cout << "......写\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
			fmutex.signal();

			writeMutex.wait();
			if(--write_count == 0)
				queue.signal();
			writeMutex.signal();
		}
	});
	reader.join();
	writer.join();
	return 0;
}
