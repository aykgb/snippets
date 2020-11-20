/*************************************************************************
    > File Name: tst-linux_timer_unsleep.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Tue 03 May 2016 06:41:47 PM CST
 ************************************************************************/

#include<iostream>
#include<unistd.h>

int jiffes = 0;

int exception_task(int timer)
{
    while(1) {
        usleep(1000);
        jiffes++;
        if(jiffes == timer) {
            printf("jiffes: %d\n", jiffes);
            jiffes = 0;
            break;
        }
    }
    return 0;
}

using namespace std;


int main()
{
    int timer = 1000;
    exception_task(timer); // 启动一个10ms的定时器
    return 0;
}

