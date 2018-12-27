#include <iostream>
#include <algorithm>
#include <vector>
#include <list>

namespace {
	struct Node {
		int start = 0;
		int len = 0;
		int used = false;
		Node(const int _start = 0, const int _len = 0)
			: start(_start), len(_len)
		{}
	};
	int level(const int request) {
		int i = 0;
		while((1 << i) < request) ++i;
		return i;
	}
}

class Buddy {
	Buddy(const Buddy&) = delete;
	Buddy(Buddy&&) = delete;
private:
	const int init;
	const int initSize;
	std::vector< std::list<Node> > buckets; 
public:
	Buddy(const int _init) : init(_init), initSize(1 << _init), buckets(_init, std::list<Node>())
	{
		this->buckets[_init - 1].emplace_back(0, this->initSize);
	}
	~Buddy() noexcept {
		try {
		} catch(...) {}
	}
	bool yhl_malloc(const int request) {
		int index = level(request);
		std::cout << "index  :  " << index << "\n";
		for(int i = index;i < this->init; ++i) {
			auto res = [&]{
				for(auto& it : this->buckets[i])
					if(it.used == false)
						return &it;
				return static_cast<Node*>(nullptr);
			};
			auto ans = res();
			if(ans) {
				if(ans->len > (1 << index)) {
					
				}
				return true;
			}
		}
		std::cout << "空间不足, 分配失败\n";
		return false;
	}
};

int main() {
	Buddy one(10);
	std::vector<int> sequence{ 500 };
	for(const auto it : sequence) {
		one.yhl_malloc(it);
	}
	return 0;
}
