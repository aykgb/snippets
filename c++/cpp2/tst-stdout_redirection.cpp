#include <iostream>
#include <sstream>
#include <streambuf>
#include <fstream>
#include <thread>
#include <future>

#include <stdio.h>
#include <unistd.h>

using namespace std;

std::mutex g_mtx;

int tst000()
{
    std::lock_guard<std::mutex> lock(g_mtx);

    std::cout << "thread tst000." << std::endl;
    // Rddirect cout.
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ofstream ofs("output.txt", std::ofstream::out | std::ofstream::app);
    if(!ofs.is_open()) {
        std::cout << "file open failed." << std::endl;
        return 1;
    }

    std::cout.rdbuf(ofs.rdbuf());

    // This goes to the string stream.
    std::cout << "tst000" << std::endl;
    std::cout << "Hello, cout." << std::endl;
    printf("Hello, printf.\n");

    // Restore old cout.
    std::cout.rdbuf(oldCoutStreamBuf);

    ofs.close();

    return 0;
}

int tst001()
{
    std::lock_guard<std::mutex> lock(g_mtx);

    std::cout << "thread tst001." << std::endl;

    FILE *fp = freopen("output.txt", "a+", stdout);
    if(fp == NULL) {
        std::cout << "error: freopen failed." << std::endl;
    }
    std::cout << "tst001" << std::endl;
    std::cout << "Hello, cout." << std::endl;
    printf("Hello, printf.\n");

    dup2(fileno(fp), 1);
    fclose(fp);

    return 0;
}

int tst002()
{
    std::lock_guard<std::mutex> lock(g_mtx);

    std::cout << "thread tst002." << std::endl;

    int stdout_dupfd = dup(1);
    // std::cout << "#### stdout_dupfd: " << stdout_dupfd << std::endl;
    FILE *tmp_out = fopen("output.txt", "a+");
    // std::cout << "#### fileno(tmp_out): " << fileno(tmp_out) << std::endl;
    dup2(fileno(tmp_out), 1);

    std::cout << "tst002" << std::endl;
    std::cout << "Hello, cout." << std::endl;
    printf("Hello, printf.\n");

    fflush(stdout);
    fclose(tmp_out);

    dup2(stdout_dupfd, 1);
    close(stdout_dupfd);

    return 0;
}

int main()
{
    std::cout << "main thread start..." << std::endl;

    std::future<int> fut = std::async(tst002);

    if(fut.get() == 0) {
        std::cout << "fut SUCCESS." << std::endl;
    } else {
        std::cout << "fut FAIL." << std::endl;
    }

    std::cout << "main thread end." << std::endl;

    return 0;
}
