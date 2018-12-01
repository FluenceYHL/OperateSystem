#include <sys/unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <iostream>
#include "scopeguard.h"

namespace {
	void logCall(const char* message) {
		std::cout << message << "\n";
	}
	void errorCall(const char* message) {
		std::cerr << message << "\n";
	}
}

const char* getMode(unsigned short mode) {
	if(mode & 0400) 
		return "拥有者可读";
	else if(mode & 0200) 
		return "拥有者可写";
	else if(mode & 0040)
		return "组可读";
	else if(mode & 0020)
		return "组可写";
	else if(mode & 0004)
		return "全局可读";
	else if(mode & 0002)
		return "全局可写";
	return "其他";
}

int main(int argc, char const *argv[])
{
	if(argc not_eq 2) {
		errorCall("参数个数错误"); exit(-1);
	}
	const int id = std::atoi(argv[1]);
	struct shmid_ds one;
	int info = shmctl(id, IPC_STAT, &one);
	if(info < 0) {
		errorCall("不能获取共享内存段"); exit(-3);
	}
	std::cout << "共享内存 ID  :  " << id << "\n";
	std::cout << "关键字       :  " << one.shm_perm.__key << "\n";
	std::cout << "模式         :  " << getMode(one.shm_perm.mode) << "\n";
	std::cout << "用户 ID      :  " << one.shm_perm.uid << "\n";
	std::cout << "内存大小     :  " << one.shm_segsz << "\n";
	std::cout << "附加的数目   :  " << one.shm_nattch << "\n";
	return 0;
}