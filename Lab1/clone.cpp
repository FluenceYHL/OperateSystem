#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include "scopeguard.h"

namespace {
	struct Node{
		int lchild = -1, rchild = 1;
	};

	static constexpr int len = 10000;
	void *pstack = malloc(len);
	YHL::ON_SCOPE_EXIT([&]{
		free(pstack);
		std::cout << "\n子进程堆栈空间已释放\n";
	});
	// 栈内存变量
	int num = 0;
	// 容器的特殊堆内存
	std::vector<int> arr{ 1, 3, 6 };
	// 文件　I/O
	std::ofstream out("test.txt");
	// 堆内存变量
	Node *root = new Node();
	YHL::ON_SCOPE_EXIT([&]{
		if(root) {
			std::cout << "\nmain 退出, root 申请的内存已释放\n";
			delete root; root = nullptr;
		} 
	});

	void print(const int num, const std::vector<int>& arr,
			   Node *const root) {
		std::cout << "\t栈变量\tnum  =  " << num << "\n";
		std::cout << "\t堆变量\troot =  " << root->lchild << " , " << root->rchild << "\n";
		std::cout << "\t容器(特殊堆变量) : ";
		for(const auto& it : arr)
			std::cout << it << " ";
		std::cout << "\n";
	}
	
	int clone_test(void*) {
		print(num, arr, root);
		printf(out.is_open() ? "\t子进程中文件输出流　out 打开\n" : "\t子进程中文件输出流　out　已关闭");
	}
}

int main() {
	if(!pstack) {
		std::cerr << "子进程申请系统堆栈失败\n"; exit(-1);
	}
	auto handle = clone(&clone_test, (char *)pstack + len, CLONE_VM|CLONE_FS|CLONE_FILES, 0);
	if(handle == -1) {
		std::cerr << "clone 失败\n"; exit(-2);
	}

	std::cout << "\n在父进程中修改变量\n";
	num = 10000;
	for(int i = 0;i < 10; ++i) 
		arr.emplace_back(i + 1);
	root->lchild = -100, root->rchild = 100;
	out.close();

	std::cout << "父进程退出\n";
	return 0;
}

// detached, main 先退出