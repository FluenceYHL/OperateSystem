#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>

int main() {
	srand(time(nullptr));
	int cur = 1;
	constexpr int page = 256;
	constexpr int initSize = 64 * page;
	auto getRand = [](int l, int r) {
		l = std::abs(l), r = std::abs(r);
		if(l == r) return l;
		if(l > r) std::swap(l, r);
		return l + rand() % (r - l);
	};
	std::vector<int> sequence{ cur = rand() % (initSize - 1) };
	for(int i = 1;i < initSize; ++i) {
		auto r = rand() % 10;
		if(r < 7) {
			if(cur >= initSize - 1) 
				cur -= rand() % (page);
			sequence.emplace_back(++cur);
		}
		else if(r < 8) {
			auto l = rand() % 20;
			if(l < 17) 
				sequence.emplace_back(cur = getRand(cur - (page << 1), cur));
			else if(l < 19) 
				sequence.emplace_back(cur = getRand(cur - (page << 2), cur - (page << 1)));
			else 
				sequence.emplace_back(cur = getRand(0, cur - (page << 2)));
		}
		else {
			auto l = rand() % 20;
			if(l < 17) 
				sequence.emplace_back(cur = std::min(initSize - (page << 1), getRand(cur, cur + (page << 1))));
			else if(l < 19) 
				sequence.emplace_back(cur = std::min(initSize - (page << 1), getRand(cur + (page << 1), cur + (page << 2))));
			else 
				sequence.emplace_back(cur = std::min(initSize - (page << 1), getRand(cur + (page << 2), initSize)));
		}
	}
	std::ofstream out("./sequence(3).txt", std::ios::trunc);
	std::cout << "initSize : " << initSize << "\n";
	for(const auto it : sequence) 
		out << it << " ";
	out.close();
	return 0;
}