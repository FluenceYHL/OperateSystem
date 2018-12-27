#include <iostream>
#include <algorithm>
#include <vector>
#define all(x) x.begin(), x.end()

namespace {
	struct Node {
		int start = 0;
		int len = 0;
		int used = false;
		Node(const int _start = 0, const int _len = 0, bool _used = false)
			: start(_start), len(_len), used(_used)
		{}
		void display(const char *message = "") const {
			std::cout << "(" << start << ", " << start + len << ")  " << message << "\n";
		}
		std::pair<int, int> get() const {
			return std::pair<int, int>(start, len);
		}
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
	std::vector< std::vector<Node> > buckets; 

public:
	Buddy(const int _init) : init(_init + 1), initSize(1 << _init), buckets(_init + 1, std::vector<Node>())
	{
		this->buckets[_init].emplace_back(0, this->initSize);
	}

	~Buddy() noexcept {
		try {
		} catch(...) {}
	}

	void display() {
		std::cout << "\n";
		for(int i = 0;i < this->init; ++i) {
			std::cout << i << "  :  ";
			for(const auto& it : this->buckets[i])
				std::cout << it.used << " (" << it.start << ", " << it.start + it.len << ")\t";
			std::cout << "\n";
		}
	}

	std::pair<int, int> yhl_malloc(const int request, bool show=false) {
		if(request <= 0) {
			std::cout << "请求大小非法\n"; return std::make_pair<int, int>(-1, -1);
		}
		int index = level(request);
		// std::cout << "index  :  " << index << "\n";
		for(int i = index;i < this->init; ++i) {
			auto t = std::find_if(all(this->buckets[i]), [&](const Node& it){ return it.used == false; });
			if(t not_eq this->buckets[i].end()) {
				int pos = std::distance(this->buckets[i].begin(), t);
				if(i == index) {
					this->buckets[i][pos].used = true;
					this->buckets[i][pos].display("整个被分配出去\n");
					if(show == true) this->display();
					return this->buckets[i][pos].get();
				} else {
					// std::cout << "i  :  " << i << "\tpos  :  " << pos << "\n";
					this->buckets[i][pos].display("要分裂\n");
					auto start = this->buckets[i][pos].start;
					auto len = this->buckets[i][pos].len;
					// std::cout << start << "\t" << len << "\n";
					this->buckets[i].erase(this->buckets[i].begin() + pos);
					while(--i >= index) {
						// std::cout << i << "  start  :  " << start << "\t" << (1 << i) << "\n";
						this->buckets[i].emplace_back(start, (1 << i), false);
						start += (1 << i);
					}
					this->buckets[index].emplace_back(start, (1 << (i + 1)), true);
					if(show == true) this->display();
					return std::make_pair<int, int>(std::move(start), (1 << (i + 1)));
				}
			}
		}
		std::cout << "空间不足, 分配失败\n";
		return std::make_pair<int, int>(-2, -2);
	}

	bool yhl_release(const int l, const int r) {
		if(l < 0 || r <= 0 || l + r > this->initSize) {
			std::cout << "内存段非法, 无法释放\n"; return false;
		}
		std::cout << "请求释放　 (" << l << ", " << l + r << ")\n"; 
		int index = level(r);
		std::cout << "index  :  " << index << "\n";
		auto t = std::find_if(all(this->buckets[index]), [&](const Node& it){ return it.start == l; });
		if(t not_eq this->buckets[index].end()) {
			auto pos = std::distance(this->buckets[index].begin(), t);
			if(this->buckets[index][pos].used == false) {
				std::cout << "该内存端暂未分配\n";
				return false;
			} 
			else {
				this->buckets[index][pos].used = false;
				// t = std::find_if(all(this->buckets[index]), [&](const Node& it){ return std::abs(it.start - l) == r; });
				// std::cout << "找到了\n";
				// t->display();
				// // 如果找到地址连续的块, 而且是空闲的块, 就合并
				// if(t not_eq this->buckets[index].end() and t->used == false) {
				// 	std::cout << "可以开始合并\n";
				// 	// 这里第一次合并
				// 	int des = std::min(l, t->start);
				// 	std::cout << "合并成　" << des + (r << 1);
				// 	// 删掉两个子块
				// 	this->buckets[index].erase(this->buckets[index].begin() + pos);
				// 	this->buckets[index].erase(t);
				// 	// 生成一个新块
				// 	this->buckets[index + 1].emplace_back(des, (r << 1));
				// 	this->display();
				// 	for(int i = index + 1;i <= this->init; ++i) {
				// 		t = std::find_if(all(this->buckets[i]), [&](const Node& it){ return std::abs(it.start - des) == (1 << i); });
				// 		if(t not_eq this->buckets[i].end() and t->used == false) {
				// 			des = std::min(des, t->start);
				// 			std::cout << "继续合并\n";
				// 			this->buckets[i].erase(this->buckets[i].end() - 1);
				// 			this->buckets[i].erase(t); 
				// 			this->buckets[i + 1].emplace_back(des, (1 << (i + 1)));
				// 			this->display();
				// 			// 这里记得要　break, 设置一个　flag
				// 		}
				// 		else break;
				// 	}
				// }
				auto des = l;
				for(int i = index;i <= this->init; ++i) {
					t = std::find_if(all(this->buckets[i]), [&](const Node& it){ return std::abs(it.start - des) == (1 << i); });
					if(t not_eq this->buckets[i].end() and t->used == false) {
						des = std::min(des, t->start);
						std::cout << "继续合并\n";
						this->buckets[i].erase(this->buckets[i].end() - 1);
						this->buckets[i].erase(t); 
						this->buckets[i + 1].emplace_back(des, (1 << (i + 1)));
						this->display();
						// 这里记得要　break, 设置一个　flag
					}
					else break;
				}
				return true;
			}
		} 
		std::cout << "不存在该单独内存段" << "\n";
		return false;
	}
};

int main() {
	Buddy one(10);
	std::vector< std::pair<int, int> > segment;
	std::vector<int> sequence{ 500, -1, 1280, 120, 30, 40, 200, 129, 1 };
	for(const auto it : sequence) {
		std::cout << "\n请求空间　　:  " << it << "\t";
		segment.emplace_back(one.yhl_malloc(it));
	}
	// 开始随机释放
	std::cout << "\n开始释放　:  \n\n";
	one.yhl_release(351, 1);
	return 0;
}
