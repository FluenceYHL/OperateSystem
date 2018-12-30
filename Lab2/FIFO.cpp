/*********************************************************************************
  *Copyright(C),Your Company
  *FileName:     FIFO.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:         2018.12.18
  *Description:  模拟调页算法　FIFO
  *Compile:      g++ -std=c++14 FIFO.cpp -o FIFO -lpthread
  *Function List:  
  *History:  
     1.Date:         2018.12.18
       Author:       刘畅
       Modification: 完成了基本功能
       Problem:     
**********************************************************************************/
#include "gnuplot.hpp"
#include <algorithm>
#define all(x) x.begin(), x.end()

void FIFO(const std::vector<int>& sequence, const int frame) {
	int fault = 0;
	std::vector<int> buffer(frame, -1);
	for(const auto it : sequence) {
		if(std::find(all(buffer), it) == buffer.end()) {
			buffer[fault % frame] = it;
			++fault;
		}
	}
	const int len = sequence.size();
	std::cout << "\nFIFO\n";
	std::cout << "共　   : " << len << "\n";
	std::cout << "页错误　: " << fault << "\n";
	std::cout << "命中率　: " << (len - fault) * 1.0 / len << "\n";
}

void LFU(const std::vector<int>& sequence, const int frame) {
	int fault = 0;
	std::vector<int> buffer(frame, -1);
	std::vector<int> cnt(frame, 0); 
	for(const auto it : sequence) {
		auto pos = std::distance(buffer.begin(), std::find(all(buffer), it)); 
		if(pos == frame) {
			++fault;
			// 现在开始找计数最小的，替换出去
			pos = std::distance(cnt.begin(), std::min_element(all(cnt)));
			buffer[pos] = it;
			cnt[pos] = 1;
		} else {
			++cnt[pos];
		}
	}
	const int len = sequence.size();
	std::cout << "\nLFU\n";
	std::cout << "共　   : " << len << "\n";
	std::cout << "页错误　: " << fault << "\n";
	std::cout << "命中率　: " << (len - fault) * 1.0 / len << "\n";
}

void LRU(const std::vector<int>& sequence, const int frame) {
	int fault = 0;
	std::vector<int> buffer(frame, -1);
	std::vector<int> cnt(frame, 0); 
	for(const auto it : sequence) {
		for(auto &it : cnt)
			it += 1;
		auto pos = std::distance(buffer.begin(), std::find(all(buffer), it)); 
		if(pos == frame) {
			++fault;
			// 现在开始找计数最大的，替换出去
			pos = std::distance(cnt.begin(), std::max_element(all(cnt)));
			buffer[pos] = it;
		}
		cnt[pos] = 0;
	}
	const int len = sequence.size();
	std::cout << "\nLRU\n";
	std::cout << "共　   : " << len << "\n";
	std::cout << "页错误　: " << fault << "\n";
	std::cout << "命中率　: " << (len - fault) * 1.0 / len << "\n";
}

void Optimal(const std::vector<int>& sequence, const int frame) {
	std::unordered_map<int, std::vector<int> > index;
	const int len = sequence.size();
	for(int i = 0;i < len; ++i)
		index[sequence[i]].emplace_back(i);
	int fault = 0;
	std::vector<int> buffer(frame, -1);
	std::unordered_map<int, int> book;
	for(int i = 0;i < len; ++i) {
		auto value = sequence[i];
		if(book[value] == 0) {
			int late = -1;
			int M = -1e5;
			for(int t = 0;t < frame; ++t) {
				int sub = buffer[t];
				int distance = 0, j = 0;
				auto ans = std::find_if(all(index[sub]), [i](const int x){ return x > i; });
				if(ans == index[sub].end())
					distance = len;
				else 
					distance = *ans;
				if(distance > M) 
					M = distance, late = t;
			}
			book[buffer[late]] = 0;
			buffer[late] = value;
			book[value] = 1;
			++fault;
		}
	}
	std::cout << "\nOptimal\n";
	std::cout << "共　   : " << len << "\n";
	std::cout << "页错误　: " << fault << "\n";
	std::cout << "命中率　: " << (len - fault) * 1.0 / len << "\n";
}

int main() {
	std::ifstream in("./sequence.txt");
	std::vector<int> sequence;
	int cur;
	while(in >> cur) 
		sequence.emplace_back(cur);
	in.close();
	for(int frame = 4;frame <= 31; ++frame) {
		FIFO(sequence, frame);
		LRU(sequence, frame);
		LFU(sequence, frame);
		Optimal(sequence, frame);
	}
	return 0;
}


// const int frame = 3;
// std::vector<int> sequence{7, 2, 3, 1, 2, 5, 3, 4, 6, 7, 7, 1, 0, 5, 4, 6, 2, 3, 0, 1};
// std::vector<int> sequence{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
// std::vector<int> sequence{1, 2, 3, 4, 5, 3, 4, 1, 6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 5, 4, 2};