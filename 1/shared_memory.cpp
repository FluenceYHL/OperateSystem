#include <sys/unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <iostream>
#include "scopeguard.h"

namespace {
	void logCall(const char* message) {
		std::cout << message << "\n";
	}
	void errorCall(const char* message) {
		std::cerr << message << "\n";
	}
	constexpr long long MAX_SIZE = 20;
}

struct sharedData {
	long long sequence[MAX_SIZE];
	int len;
};

int main(int argc, char const *argv[])
{
	if(argc not_eq 2) {
		errorCall("参数个数错误"); exit(-1);
	}
	const int n = std::atoi(argv[1]);
	if(n > MAX_SIZE) {
		errorCall("序列太长 !"); exit(-2);
	}

	int shmID = shmget(IPC_PRIVATE, sizeof(sharedData), IPC_CREAT | 0666); 
	if(shmID < 0) {
		errorCall("创建共享内存失败"); exit(-3);
	}
	std::cout << "共享内存编号 shmID  :  " << shmID << "\n";
	sharedData *one = static_cast<sharedData*>(shmat(shmID, nullptr, 0));
	YHL::ON_EXIT_SCOPE([&]{
		shmctl(shmID, IPC_RMID, nullptr);
		logCall("程序结束, 共享内存被释放");
	});
	if(one == (void *)-1) {
		errorCall("链接地址失败"); exit(-4);
	}

	one->len = n;
	pid_t pid = fork();
	if(pid < 0) {
		errorCall("fork 失败"); exit(-5);
	}
	if(pid == 0) {
		logCall("child 进程启动");
		long long a = 0, b = 1, c =1;
		int index = -1;
		one->sequence[++index] = a;
		one->sequence[++index] = b;
		for(int i = 0;i < n - 2; ++i) {
			c = a + b;
			a = b;
			b = c;
			one->sequence[++index] = c;
		}
		shmdt((void*)one);
	}
	else {
		wait(nullptr);
		for(int i = 0;i < one->len; ++i) 
			std::cout << one->sequence[i] << "\n";
		logCall("父进程退出");
		shmdt((void*)one);
	}
	return 0;
}


/*
#include <sys/unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <iostream>
#include "scopeguard.h"

namespace {
	void logCall(const char* message) {
		std::cout << message << "\n";
	}
	void errorCall(const char* message) {
		std::cerr << message << "\n";
	}
	constexpr long long MAX_SIZE = 1e6;
}

struct sharedData {
	long long sequence[MAX_SIZE];
	int len;
};

int main(int argc, char const *argv[])
{
	int shmID = shmget(IPC_PRIVATE, sizeof(sharedData), IPC_CREAT | 0666); 
	if(shmID < 0) {
		errorCall("创建共享内存失败"); exit(-2);
	}
	std::cout << "共享内存编号 shmID  :  " << shmID << "\n";
	sharedData *one = static_cast<sharedData*>(shmat(shmID, nullptr, 0));
	YHL::ON_EXIT_SCOPE([&]{
		shmctl(shmID, IPC_RMID, nullptr);
		logCall("程序结束, 共享内存被释放");
	});
	if(one == (void *)-1) {
		errorCall("链接地址失败"); exit(-3);
	}
	{
		int n;
		std::cin >> n;
		one->len = n;
		pid_t pid = fork();
		if(pid < 0) {
			errorCall("fork 失败"); exit(-4);
		}
		if(pid == 0) {
			logCall("child 进程启动");
			long long a = 0, b = 1, c =1;
			int index = -1;
			one->sequence[++index] = a;
			one->sequence[++index] = b;
			for(int i = 0;i < n - 2; ++i) {
				c = a + b;
				a = b;
				b = c;
				one->sequence[++index] = c;
			}
			shmdt((void*)one);
		}
		else {
			wait(nullptr);
			for(int i = 0;i < one->len; ++i) 
				std::cout << one->sequence[i] << "\n";
			logCall("父进程退出");
			shmdt((void*)one);
		}
	}
	return 0;
}
*/