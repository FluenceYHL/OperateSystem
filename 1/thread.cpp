#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

namespace {
	bool is_prime(const int n) {
		if(n < 2 or (n != 2 and n % 2 == 0)) 
			return 0 ; 
		for(int i = 3;i <= sqrt(n); i += 2) 
			if(n % i == 0) 
				return 0 ; 
		return 1 ; 
	}
}

int main(int argc, char const *argv[])
{
	if(argc not_eq 2) {
		std::cout << "参数个数不匹配\n";
		exit(-1);
	}
	const int n = std::atoi(argv[1]);
	std::vector<long long> sequence;
	std::thread([&]{
		for(int i = 0;i <= n; ++i) {
			if(is_prime(i))
				sequence.emplace_back(i);
		}
		for(const auto it : sequence) 
			std::cout << it << "\n";
	}).join();
	return 0;
}

/*
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char const *argv[])
{
	if(argc not_eq 2)
		exit(-1);
	const int n = std::atoi(argv[1]);
	std::vector<long long> sequence;
	std::thread([&]{
		long long a = 0, b = 1, c =1;
		sequence.emplace_back(a);
		sequence.emplace_back(b);
		for(int i = 0;i < n - 2; ++i) {
			c = a + b;
			a = b;
			b = c;
			sequence.emplace_back(c);
		}
	}).join();
	std::cout << "线程结束\n";
	for(const auto it : sequence) 
		std::cout << it << "\n";
	return 0;
}
*/