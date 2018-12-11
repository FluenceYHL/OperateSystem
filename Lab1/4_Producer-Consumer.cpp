/*********************************************************************************
  *Copyright(C),Your Company
  *FileName:     Producer-Consumer.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:         2018.11.29
  *Description:  使用 C++11 解决生产者-消费者问题
  *Compile:      g++ -std=c++14 Producer-Consumer.cpp -o pc -lpthread
  *Function List:  
  *History:  
**********************************************************************************/
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
			++count;
			int cur = count;
			std::cout << "生产者　push\t" << cnt + 1 << "\t当前缓冲区　　:  " << cur << "\n";
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
			--count;
			int cur = count;
			std::cout << "消费者　pop\t" << buffer[out] << "\t当前缓冲区　　:  " << cur << "\n";
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