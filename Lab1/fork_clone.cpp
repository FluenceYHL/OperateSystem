#include <iostream>
#include <vector>
#include <sys/unistd.h>
#include <sys/wait.h>
#include "scopeguard.h"

namespace {
	struct Node{
		int lchild = -1, rchild = 1;
	};

	void print(const int num, const std::vector<int>& arr,
			   Node *const root) {
		std::cout << "\t栈变量  num  =  " << num << "\n";
		std::cout << "\t堆变量　　root =  " << root->lchild << " , " << root->rchild << "\n";
		std::cout << "\t容器(特殊堆变量) : ";
		for(const auto& it : arr)
			std::cout << it << " ";
		std::cout << "\n";
	}
}

int main() {
	int num = 0;
	std::vector<int> arr{ 1, 3, 6 };
	Node *root = new Node();
	YHL::ON_SCOPE_EXIT([&]{
		if(root) {
			std::cout << "main 退出, root 申请的内存已释放\n";
			delete root; root = nullptr;
		} 
	});
	
	pid_t pid = fork();
	if(pid < 0) {
		std::cout << "fork 子进程失败\n"; exit(-1);
	}
	else if(pid == 0) {
		std::cout << "子进程启动 :\n";
		num += 10;
		for(int i = 0;i < 10; ++i) 
			arr.emplace_back(i + 1);
		root->lchild = -100, root->rchild = 100;
		std::cout << "\n在子进程中修改变量 : \n";
		print(num, arr, root);
	}
	else {
		std::cout << "父进程等待子进程\n";
		wait(nullptr);
		std::cout << "\n在父进程中的原始变量　　:  \n";
		print(num, arr, root);
	}
	std::cout << "父进程退出\n";
	return 0;
}


/*
比较fork（）和clone()的功能，利用fork（）生成子进程
（验证变量之间不能共享）；
用clone()生成线程（设置CLONE_FS，CLONE_VM，CLONE_FILES）
并验证其共享。
*/