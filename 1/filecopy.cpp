#include <sys/unistd.h>
#include <sys/shm.h>
#include <wait.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "scopeguard.h"

namespace {
	void logCall(const char* message) {
		std::cout << message << "\n";
	}
	void errorCall(const char* message) {
		std::cerr << message << "\n";
	}
	constexpr int MAX_SIZE = 1e3;
}

int main(int argc, char *argv[])
{
	if(argc not_eq 3) {
		errorCall("参数个数不匹配");
		exit(-1);
	}
	auto src = argv[1];
	auto des = argv[2];
	if(!src or !des) {
		errorCall("输入格式有误"); exit(-2);
	}
	int pipeMode[2];
	int pipeID = pipe(pipeMode);
	if(pipeID not_eq 0) {
		errorCall("创建管道失败"); exit(-3);
	}

	char ch[1];
	pid_t pid = fork();
	if(pid < 0) {
		errorCall("fork 失败"); exit(-5);
	}
	if(pid == 0) {
		logCall("child 进程启动");
		std::ifstream in(src);
		YHL::ON_EXIT_SCOPE([&]{ in.close(); });
		while(true) {
			ch[0] = in.get();
			if(in.eof()) break;
			write(pipeMode[1], ch, 1);
		}
		ch[0] = '\0';
		write(pipeMode[1], ch, 1);
	}
	else {
		std::ofstream out(des, std::ios::trunc);
		YHL::ON_EXIT_SCOPE([&]{ out.close(); });
		while(read(pipeMode[0], ch, 1)) {
			if(ch[0] == '\0')
				break;
			std::cout << ch;
			out << ch[0];
		}
		wait(nullptr);
		logCall("\n父进程退出");
	}
	close(pipeMode[0]);
	close(pipeMode[1]);
	logCall("程序结束，管道已关闭");
	return 0;
}


/*
#include <sys/unistd.h>
#include <sys/shm.h>
#include <wait.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "scopeguard.h"

namespace {
	void logCall(const char* message) {
		std::cout << message << "\n";
	}
	void errorCall(const char* message) {
		std::cerr << message << "\n";
	}
	constexpr int MAX_SIZE = 1e3;
}

int main(int argc, char *argv[])
{
	// if(argc not_eq 3)
	// 	exit(-1);
	// auto src = argv[1];
	// auto des = argv[2];
	// if(!src or !des) {
	// 	errorCall("输入格式有误"); exit(-2);
	// }

	const char* src = "numbers.txt";
	const char* des = "result.txt";

	int pipeMode[2];
	int pipeID = pipe(pipeMode);
	if(pipeID not_eq 0) {
		errorCall("创建管道失败"); exit(-3);
	}
	YHL::ON_EXIT_SCOPE([&]{
		close(pipeMode[0]);
		close(pipeMode[1]);
	});


	// **************************************************
	int shmID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666); 
	if(shmID < 0) {
		errorCall("创建共享内存失败"); exit(-3);
	}
	std::cout << "共享内存编号 shmID  :  " << shmID << "\n";
	int *one = static_cast<int*>(shmat(shmID, nullptr, 0));
	if(one == (void *)-1) {
		errorCall("链接地址失败"); exit(-4);
	}
	// **************************************************

	char ch[1];
	pid_t pid = fork();
	if(pid < 0) {
		errorCall("fork 失败"); exit(-5);
	}
	if(pid == 0) {
		logCall("child 进程启动");
		std::ifstream in(src);
		YHL::ON_EXIT_SCOPE([&]{ in.close(); });
		while(true) {
			ch[0] = in.get();
			if(in.eof()) break;
			std::cout << ch[0];
			write(pipeMode[1], ch, 1);
			++(*one);
		}
		shmdt((void*)one);
	}
	else {
		std::ofstream out(des, std::ios::trunc);
		YHL::ON_EXIT_SCOPE([&]{ out.close(); });
		int len;
		std::cout << "one  :  " << *one << "\n";
		for(int i = 0;i < *one; ++i) {
			read(pipeMode[0], ch, 1);
			out << ch[0];
		}
		wait(nullptr);
		logCall("\n父进程退出");
		shmdt((void*)one);
		shmctl(shmID, IPC_RMID, nullptr);
		logCall("程序结束, 共享内存被释放");
	}
	return 0;
}
*/