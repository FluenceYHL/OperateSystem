// #include "gnuplot.hpp"
// #include <condition_variable>
// #include <thread>
// #include <queue>
// #include <chrono>
// #include <map>

// int main() {
// 	std::condition_variable cv_1, cv_2;
// 	std::mutex mtx;
// 	std::thread produce, consume;

// 	constexpr static int initSize = 10;
// 	int buffer[initSize];
// 	int count = 0;
// 	int in = 0, out = 0;

// 	produce = std::thread([&]{
// 		do {
// 			static int cnt = 0;
// 			std::unique_lock<std::mutex> lck(mtx);
// 			cv_1.wait(lck, [&]{
// 				return count < initSize;
// 			});
// 			std::cout << "push\t" << cnt + 1 << "\t当前缓冲区　　:  " << count << "\n";
// 			++count;
// 			buffer[in] = ++cnt;
// 			in = (in + 1) % initSize;
// 			if(count not_eq 0)
// 				cv_2.notify_one();
// 			std::this_thread::sleep_for(std::chrono::milliseconds(50));
// 		} while(1);
// 	});
// 	consume = std::thread([&]{
// 		do {
// 			std::unique_lock<std::mutex> lck(mtx);
// 			cv_2.wait(lck, [&count]{
// 				return count > 0;
// 			});
// 			std::cout << buffer[out] << "\tpop" << "\t当前缓冲区　　:  " << count - 1 << "\n";
// 			--count;
// 			out = (out + 1) % initSize;
// 			if(count not_eq initSize)
// 				cv_1.notify_one();
// 			std::this_thread::sleep_for(std::chrono::milliseconds(20));
// 		} while(1);
// 	});
// 	consume.join();
// 	produce.join();
// 	return 0;
// }


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
			std::cout << "生产者　push\t" << cnt + 1 << "\t当前缓冲区　　:  " << count << "\n";
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
			std::cout << "消费者　pop\t" << buffer[out] << "\t当前缓冲区　　:  " << count - 1 << "\n";
			--count;
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