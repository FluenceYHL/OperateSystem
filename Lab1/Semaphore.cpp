#include "gnuplot.hpp"
#include <condition_variable>
#include <functional>
#include <thread>
#include <queue>
#include <chrono>
#include <atomic>
#include <map>

class Semaphore {
public:
	void wait() {
		std::unique_lock<std::mutex> lck(this->mtx);
		--this->count;
		this->cv.wait(lck, [this]{
			return this->count >= 0;
		});
	}
	void signal() {
		std::unique_lock<std::mutex> lck(this->mtx);
		if(++count <= 0) 
			this->cv.notify_all();
	}
private:
	std::mutex mtx;
	std::condition_variable cv;
	int count = 10;
};

int main() {
	std::mutex read_mtx;
	std::unique_lock<std::mutex> lck(read_mtx, std::defer_lock);
	Semaphore sem;
	std::atomic<int> count{ 0 };

	static constexpr int initSize = 20;
	srand(unsigned(time(nullptr)));
	std::thread flows[initSize];
	int cnt = -1;

	for(int i = 0;i < initSize; ++i) {
		if((rand() % 2) & 1) {
			flows[++cnt] = std::thread([&]{
				std::unique_lock<std::mutex> lck2(read_mtx); // 这里获取不到互斥量
				std::cout << count << " 个写者\t\t读文件\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			});
		} 
		else {
			flows[++cnt] = std::thread([&]{
				sem.wait();
				if(++count == 1) // 如果有写者, 读者不在读，锁住互斥量
					lck.lock();  // 如果读者正在读, 互斥量被读者锁占有, 写者获取不到互斥量
				sem.signal();

				std::cout << count << " 个写者\t\t写文件\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

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