#include <iostream>
#include <algorithm>
#include <vector>

struct Node {
	int weight = 0;
	int used = 0;
	std::vector<int> ch;
	Node(const int _weight = 0, const std::vector<int> _ch = {0, 0, 0}) 
		: weight(_weight), ch(std::move(_ch))
	{}
	void display() const {
		std::cout << "weight  :  " << this->weight << "\n";
		std::cout << "lchild  :  " << this->ch[0] << "\n";
		std::cout << "rchild  :  " << this->ch[1] << "\n";
		std::cout << "parent  :  " << this->ch[2] << "\n";
	}
};

constexpr int initSize = 1 << 10;
std::vector<Node> tree(initSize << 1, Node());
int root = 1;

int main() {
	tree[root] = Node(initSize);
	tree[root].display();
	return 0;
}