#include <sys/unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <cstdio>
#include <iostream>
#include "scopeguard.h"

namespace {
	void logCall(const char* message) {
		std::cout << message << "\n";
	}
	void errorCall(const char* message) {
		std::cerr << message << "\n";
	}
	void outputLL(const long long data) {
		std::cout << data << "\n";
	}
}

int main(int argc, char const *argv[])
{
	if(argc not_eq 2)
		exit(-1);
	const int n = std::atoi(argv[1]);
	pid_t pid = fork();
	if(pid < 0) {
		errorCall("fork 失败"); exit(-2);
	}
	if(pid == 0) {
		logCall("child 进程启动");
		long long a = 0, b = 1, c =1;
		outputLL(a);
		outputLL(b);
		for(int i = 0;i < n - 2; ++i) {
			c = a + b;
			a = b;
			b = c;
			outputLL(c);
		}
	}
	else {
		wait(NULL);
		logCall("父进程退出");
	}
	return 0;
}