#include "gnuplot.hpp"
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>
#include <map>

int main() {
	std::condition_variable cv_1, cv_2;
	std::mutex mtx;
	std::thread produce[30], consume[30];

	constexpr static int initSize = 10;
	int buffer[initSize];
	int count = 0;
	int in = 0, out = 0;

	for(int i = 0;i < 30; ++i) {
		produce[i] = std::thread([&]{
			static int cnt = 0;
			std::unique_lock<std::mutex> lck(mtx);
			cv_1.wait(lck, [&]{
				return count < initSize;
			});
			std::cout << "push" << '\n';
			++count;
			buffer[in] = ++cnt;
			in = (in + 1) % initSize;
			if(count not_eq 0)
				cv_2.notify_one();
		});
		consume[i] = std::thread([&]{
			std::unique_lock<std::mutex> lck(mtx);
			cv_2.wait(lck, [&count]{
				return count > 0;
			});
			std::cout << "pop" << '\n';
			--count;
			std::cout << buffer[out] << "\n";
			out = (out + 1) % initSize;
			if(count not_eq initSize)
				cv_1.notify_one();
		});
	}
	for(int i = 0;i < 30; ++i) {
		produce[i].join();
		consume[i].join();
	}
	return 0;
}