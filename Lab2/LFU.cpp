/*********************************************************************************
  *Copyright(C),Your Company
  *FileName:     LFU.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:         2018.12.18
  *Description:  模拟调页算法　LFU
  *Compile:      g++ -std=c++14 LFU.cpp -o LFU -lpthread
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
// cnt      计数, 页表对应位置未被使用的时间
void LFU(const std::vector<int>& sequence, const int frame) {
	int fault = 0;
	std::vector<int> buffer(frame, -1);
	std::vector<int> cnt(frame, 0); 
	for(const auto it : sequence) {
		auto pos = std::distance(buffer.begin(), std::find(all(buffer), it)); 
		if(pos == frame) {
			++fault;
			// 现在开始找计数最大的，替换出去
			pos = std::distance(cnt.begin(), std::min_element(all(cnt)));
			buffer[pos] = it;
			cnt[pos] = 1;
		} else {
			++cnt[pos];
		}
	}
	std::cout << "页错误　: " << fault << "\n";
}

int main() {
	const int frame = 3;
	std::vector<int> sequence{7, 2, 3, 1, 2, 5, 3, 4, 6, 7, 7, 1, 0, 5, 4, 6, 2, 3, 0, 1};
	// std::vector<int> sequence{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
	// std::vector<int> sequence{1, 2, 3, 4, 5, 3, 4, 1, 6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 5, 4, 2};
	LFU(sequence, frame);
	return 0;
}

/* 运行结果
*/