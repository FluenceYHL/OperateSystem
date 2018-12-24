/*********************************************************************************
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
#define all(x) x.begin(), x.end()

// sequence 页的引用串
// frame    帧数
// buffer   页表, 长度为　frame
// fault    当前错误个数
void Optimal(const std::vector<int>& sequence, const int frame) {
	int fault = 0;
	std::vector<int> buffer(frame, -1);
	for(auto cur = sequence.begin();cur not_eq sequence.end(); ++cur) {
		// 如果当前缺页
		if(std::find(all(buffer), *cur) == buffer.end()) {
			auto late = -1;
			auto M = -1;
			for(int i = 0;i < frame; ++i) {
				// 计算未来出现最晚的页表项
				auto dis = std::distance(cur, std::find(cur, sequence.end(), buffer[i]));
				if(dis > M)
					M = dis, late = i;
			}
			buffer[late] = *cur;
			++fault;
		}
	}
	std::cout << "页错误　: " << fault << "\n";
}

int main() {
	const int frame = 3;
	std::vector<int> sequence{7, 2, 3, 1, 2, 5, 3, 4, 6, 7, 7, 1, 0, 5, 4, 6, 2, 3, 0, 1};
	// std::vector<int> sequence{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
	// std::vector<int> sequence{1, 2, 3, 4, 5, 3, 4, 1, 6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 5, 4, 2};
	Optimal(sequence, frame);
	return 0;
}

/* 运行结果
13
*/