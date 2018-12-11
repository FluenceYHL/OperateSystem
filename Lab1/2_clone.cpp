/*********************************************************************************
  *Copyright(C),Your Company
  *FileName:     clone.cpp
  *Author:       刘畅
  *Version:      1.0
  *Date:         2018.11.29
  *Description:  实验验证 linux 下 clone() 生成的轻量子进程与父进程资源共享
  *Compile:      g++ -std=c++14 clone.cpp -o clone -lpthread
  *Function List:  
     1.print       打印不同资源类型的变量
     2.signal_test 尝试信号机制
     3.child_flow  子进程流, 作为 clone 的第一个参数, 在这里修改变量
  *History:  
     1.Date:         2018.11.29
       Author:       刘畅
       Modification: 完成了基本功能
       Problem:      输出不稳定，有时共享有时不共享
     2.2018.12.6 
       Author:   刘畅
       Modification: 将 FLAGS 改为 CLONE_VFORK, 解决了输出不稳定的问题
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
    // 声明各种资源类型的变量
    int num = 0;
    char *pstack = nullptr;
    std::vector<int> arr{ 1, 3, 6};
    struct Node {
        int lchild = -1, rchild = 1;
    } *root = new Node();
    std::ofstream out("test.txt");
    static constexpr int STACK_SIZE = 10000;

    // 信号函数, ctrl + C
    void signal_test(const int un_used) {
        std::cout << "捕获一个信号\n";
    }
    // 打印一些变量
    void print() {
        std::cout << "\t栈变量\tnum : " << num << "\n";
        std::cout << "\t堆变量\troot : " << root->lchild << " , " << root->rchild << "\n";
        std::cout << "\t容器变量 arr  :  "; 
        for(const auto& it : arr)
            std::cout << it << "  ";
        std::cout << "\n";
        printf(out.is_open() ? "\t文件流 out 打开\n" : "\t文件流 out 关闭\n");
    }

    int child_flow(void *) {
        num += 10;
        for(int i = 0;i < 10; ++i)
            arr.emplace_back(i + 1);
        root->lchild = -100, root->rchild = 100;
        out.close();
        std::cout << "子进程中  : \n";
        print();
        exit(0);
    }
}


int main() {
    std::cout << "初始值如下  :  \n";
    print();
    signal(SIGINT, signal_test);
    pstack = (char*)malloc(STACK_SIZE);
    if(!pstack) {
        std::cout << "clone 分配栈空间失败\n";
        exit(0);
    }
    // Scopeguard, 确保程序结束，清理操作的执行
    YHL::ON_SCOPE_EXIT([&]{
        if(pstack)
            free(pstack);
        std::cout << "\nclone() 分配的栈空间已释放\n";
        if(root) {
            delete root; root = nullptr;
        }
        if(out.is_open()) out.close();
    });
    clone(child_flow, pstack + STACK_SIZE, CLONE_VM | CLONE_VFORK, 0);
    std::cout << "父进程中  :  \n";
    print();
    return 0;
}