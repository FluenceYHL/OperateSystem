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
		--this->count;
		this->cv.wait(lck, [this]{  // 阻塞操作
			return this->count >= 0;
		});
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
	// 与线程相关
	std::mutex read_mtx;  // 互斥量      
	std::unique_lock<std::mutex> lck(read_mtx, std::defer_lock); // 这里要延迟 defer_lock
	Semaphore sem(10);        // 设置信号量，控制同时访问的线程个数
	std::atomic<int> count{ 0 };  // 改成原子变量更安全

	static constexpr int initSize = 20;
	srand(unsigned(time(nullptr)));
	std::thread flows[initSize];
	int cnt = -1;

	for(int i = 0;i < initSize; ++i) {
		if((rand() % 2) & 1) {
			// 读者线程
			flows[++cnt] = std::thread([&]{
				std::unique_lock<std::mutex> lck2(read_mtx); // 这里获取不到互斥量
				std::cout << count << " 个写者\t\t读文件\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			});
		} 
		else {
			// 写者线程
			flows[++cnt] = std::thread([&]{
				sem.wait();
				if(++count == 1) // 如果有写者, 读者不在读，锁住互斥量
					lck.lock();  // 如果读者正在读, 互斥量被读者锁占有, 写者获取不到互斥量
				sem.signal();

				std::cout << count << " 个写者\t\t写文件\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(40));

				sem.wait();
				if(--count == 0)  // 如果无写者, 将互斥量让给读者
					lck.unlock();
				sem.signal();    // 信号量保护　count
			});
		}
	}
	for(int i = 0;i < initSize; ++i)
		flows[i].join();
	return 0;
}