#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <queue>
#include <condition_variable>

#include <string.h>
#include <stdlib.h>

const int MAX_READ_BUF_SIZE = 1024;
char buf[MAX_READ_BUF_SIZE];
std::mutex mtx;

void echo(const char *contents, int len)
{
    std::cout.write(contents, len);
    std::cout.put('\n');
}

void run()
{
    while(true) {
        std::unique_lock<std::mutex> lck(mtx);
        std::cout << "$myshell>";
        std::cin.getline(buf, MAX_READ_BUF_SIZE);
    }
}

int main()
{
    std::thread thread_run(run);

    while(true) {
        if(strlen(buf) == 0) continue;

        std::unique_lock<std::mutex> lck(mtx);
        std::string cmd(buf);
        if(cmd == "echo") {
            echo(cmd.c_str(), cmd.length());
        }

        if(cmd == "quit" || cmd == "q") {
            exit(0);
        }

        memset(buf, MAX_READ_BUF_SIZE, 1);
    }

    thread_run.join();

    return 0;
}
