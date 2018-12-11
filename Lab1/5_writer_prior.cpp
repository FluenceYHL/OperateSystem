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
	Semaphore mutex(1);        // 设置信号量，控制同时访问的线程个数
	Semaphore rw(1);
	Semaphore w(1);
	// std::atomic<int> count{ 0 };  // 改成原子变量更安全
	int count = 0;

	static constexpr int initSize = 20;
	srand(unsigned(time(nullptr)));
	std::thread flows[initSize];
	int cnt = -1;

	for(int i = 0;i < initSize; ++i) {
		if((rand() % 2) & 1) {
			// 读者线程
			flows[++cnt] = std::thread([&]{
				w.wait();
				rw.wait();
				int cur = count;
				std::cout << "读者  :  " << cur << "个\t\t" << "写文件\n";
				rw.signal();
				w.signal();
			});
		} 
		else {
			// 写者线程
			flows[++cnt] = std::thread([&]{
				w.wait();
				mutex.wait();
				if(++count == 1)
					rw.wait();
				// ++count;
				mutex.signal();
				w.signal();

				int cur = count;
				std::cout << "读者  :  " << cur << "个\t\t" << "读文件\n";

				mutex.wait();
				if(--count == 0)
					rw.signal();
				mutex.signal();
			});
		}
	}
	for(int i = 0;i < initSize; ++i)
		flows[i].join();
	return 0;
}