/*********************************************************************************
  *Copyright(C),Your Company
  *FileName:     fork.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:     	 2018.11.29
  *Description:  实验验证 linux 下 fork() 子进程与父进程资源不共享
  *Compile:  	 g++ -std=c++14 fork.cpp -o fork -lpthread
  *Function List:  
     1.print     打印不同资源类型的变量
     2.signal_test 尝试信号机制
  *History:  
     1.Date:     2018.11.29
       Author:   刘畅
       Modification: 完成了基本功能
     2.2018.12.6 
       Author:   刘畅
       Modification: 完善了 signal 机制的使用
**********************************************************************************/
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

	void print(const int num, const std::vector<int>& arr,
			   Node *const root) {
		std::cout << "\t栈变量\tnum  =  " << num << "\n";
		std::cout << "\t堆变量\troot =  " << root->lchild << " , " << root->rchild << "\n";
		std::cout << "\t容器(特殊堆变量) : ";
		for(const auto& it : arr)
			std::cout << it << " ";
		std::cout << "\n";
	}

	void signal_test(const int un_used) {
		std::cout << "捕获一个信号, 地址是  : " << &un_used << "\n";
	}
}

int main() {
	// 栈内存变量
	int num = 0;
	// 容器的特殊堆内存
	std::vector<int> arr{ 1, 3, 6 };
	// 文件　I/O
	std::ofstream out("test.txt");
	assert(out);
	YHL::ON_SCOPE_EXIT([&]{ if(out.is_open()) out.close(); });
	// 信号处理
	auto sign = signal(SIGINT, signal_test);  // 接收 Ctrl+C
	// 堆内存变量
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
		out.close();
		std::cout << "\n在子进程中修改变量 : \n";
		print(num, arr, root);
		std::cout << "\t子进程中文件输出流　out 关闭\n";
		sleep(2);
		exit(0);
	}
	else {
		std::cout << "父进程等待子进程\n";
		wait(nullptr);
		std::cout << "\n父进程与子进程汇合\n\n在父进程中的原始变量　　:  \n";
		print(num, arr, root);
		printf(out.is_open() ? "\t父进程中文件输出流　out 打开\n" : "\t父进程中文件输出流　out　已关闭\n");
		std::cout << "父进程退出\n";
		sleep(2);
	}
	return 0;
}