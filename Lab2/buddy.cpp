#include <iostream>
#include <algorithm>
#include <vector>

struct Node {
	int start = 0;
	int len = 0;
	Node *pre, *nex;
	Node() {
		this->pre = this->nex = this;
	}
	Node(const int _start, const int _len, Node* _pre=nullptr, Node* _nex=nullptr)
		: start(_start), len(_len), pre(_pre), nex(_nex)
	{}
};

class Buddy {
	Buddy(const Buddy&) = delete;
	Buddy(Buddy&&) = delete;
private:
	const int initSize;
	std::vector<Node*> buckets; 
public:
	Buddy(const int _init) : initSize(1 << _init), buckets(_init, new Node())
	{
		for(int i = 0;i < 5; ++i)
			this->buckets[0]
	}
	~Buddy() noexcept {
		try {
			for(auto it : this->buckets) {
				auto p = it->nex;
				while(p not_eq it) {
					auto t = p;
					p = p->nex;
					t->pre->nex = t->nex;
					delete t;
					t = nullptr;
				}
				delete it; it = nullptr;
			}
		} catch(...) {}
	}
};

int main() {
	return 0;
}
