#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>

int main() {
	srand(time(nullptr));
	int cur = 1;
	std::vector<int> sequence{ 0 };
	constexpr int initSize = 16 * 1024;
	for(int i = 1;i < initSize; ++i) {
		auto r = rand() % 10;
		if(r < 7) {
			sequence.emplace_back(cur++);
		}
		else if(r < 9) {
			while(cur == 1) ++cur;
			sequence.emplace_back(cur = rand() % (cur - 1));
		}
		else {
			while(initSize - cur - 1 == 0) ++cur;
			sequence.emplace_back(cur = cur + 1 + rand() % (initSize - cur - 1));
		}
	}
	std::ofstream out("./sequence.txt", std::ios::trunc);
	for(const auto it : sequence)
		out << it << " ";
	out.close();
	return 0;
}