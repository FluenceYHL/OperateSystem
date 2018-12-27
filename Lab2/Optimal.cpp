/********************************************************************************
  *Copyright(C),Your Company
  *FileName:     Optiaml.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:         2018.12.18
  *Description:  模拟调页算法　Optimal 最优页面置换算法
  *Compile:      g++ -std=c++14 Optimal.cpp -o Optimal -lpthread
  *Function List:  
  *History:  
     1.Date:         2018.12.18
       Author:       刘畅
       Modification: 完成了基本功能
       Problem:     
**********************************************************************************/
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#define all(x) x.begin(), x.end()


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
				if(index[sub].empty() || index[sub].back() <= i) 
					distance = len;
				else {
					while(index[sub][j] <= i) ++j;
					distance = index[sub][j];
				}
				if(distance > M) 
					M = distance, late = t;
			}
			book[buffer[late]] = 0;
			buffer[late] = value;
			book[value] = 1;
			++fault;
		}
	}
	std::cout << "错误　　:  " << fault << "\n";
}

int main() {
	const int frame = 3;
	// std::vector<int> sequence{7, 2, 3, 1, 2, 5, 3, 4, 6, 7, 7, 1, 0, 5, 4, 6, 2, 3, 0, 1};
	// std::vector<int> sequence{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
	// std::vector<int> sequence{1, 2, 3, 4, 5, 3, 4, 1, 6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 5, 4, 2};
	Optimal(sequence, frame);
	return 0;
}
