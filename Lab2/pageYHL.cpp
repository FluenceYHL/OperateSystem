/*********************************************************************************
  *Copyright(C),Your Company
  *FileName:     FIFO.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:         2018.12.18
  *Description:  模拟调页算法　FIFO
  *Compile:      g++ -std=c++17 FIFO.cpp -o FIFO -lpthread
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

namespace {
	double FIFO(const std::vector<int>& sequence, const int frame) {
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
		return (len - fault) * 1.0 / len;
	}

	double LFU(const std::vector<int>& sequence, const int frame) {
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
		return (len - fault) * 1.0 / len;
	}

	double LRU(const std::vector<int>& sequence, const int frame) {
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
		return (len - fault) * 1.0 / len;
	}

	double Optimal(const std::vector<int>& sequence, const int frame) {
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
					else distance = *ans;
					if(distance > M) M = distance, late = t;
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
		return (len - fault) * 1.0 / len;
	}

	// double Optimal(const std::vector<int>& sequence, const int frame) {
	// 	int fault = 0;
	// 	std::vector<int> buffer(frame, -1);
	// 	for(auto cur = sequence.begin();cur not_eq sequence.end(); ++cur) {
	// 	// 如果当前缺页
	// 		if(std::find(all(buffer), *cur) == buffer.end()) {
	// 			auto late = -1, M = -1;
	// 			for(int i = 0;i < frame; ++i) {
	// 				auto dis = std::distance(cur, std::find(cur, sequence.end(), buffer[i]));
	// 				if(dis > M) M = dis, late = i;
	// 			}
	// 			buffer[late] = *cur;
	// 			++fault;
	// 		}
	// 	}
	// 	const int len = sequence.size();
	// 	std::cout << "Optimal : " << (len - fault) * 1.0 / len << "\n";
	// }
}

int main() {
	std::ifstream in("./sequence(2).txt");
	std::vector<int> sequence;
	int cur;
	while(in >> cur) 
		sequence.emplace_back(cur / 256);
	in.close();
	constexpr int algorithm_cnt = 4;
	constexpr int lhs = 4, rhs = 31;
	std::vector< std::vector<point> > curves(algorithm_cnt, std::vector<point>());
	for(int frame = lhs;frame <= rhs; ++frame) {
		curves[0].emplace_back(frame, FIFO(sequence, frame));
		curves[1].emplace_back(frame, LRU(sequence, frame));
		curves[2].emplace_back(frame, LFU(sequence, frame));
		curves[3].emplace_back(frame, Optimal(sequence, frame));
	}
	const std::vector<std::string> names{ "FIFO", "LRU", "LFU", "Optimal" };
	for(int i = 0;i < algorithm_cnt; ++i) {
		std::ofstream out(names[i].c_str(), std::ios::trunc);
		for(const auto& it : curves[i]) 
			out << it.features[0] << " " << it.features[1] << "\n";
		out.close();
	}
	YHL::GNU_plot plotter;
	// plotter.write("set multiplot");
	plotter.write("set key outside");
	plotter.write("set title 'curves about page-hit-rate'");
	plotter.write("set xlabel 'frame'");
	plotter.write("set ylabel 'page-hit-rate'");
	std::string commands = "plot ";
	for(int i = 0;i < algorithm_cnt; ++i) 
		commands += "'" + names[i] + "' " + "w l lt " + std::to_string(i + 1) + " lw 5,";
	commands.pop_back();
	plotter.write(commands);
	plotter.plot();
	return 0;
}
