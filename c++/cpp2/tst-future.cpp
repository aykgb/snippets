// future example
#include <cmath>
#include <iostream>             // std::cout
#include <functional>
#include <thread>
#include <future>               // std::async, std::future
#include <chrono>               // std::chrono::milliseconds
#include <exception>
#include <utility>

// a non-optimized way of checking for prime numbers:
using namespace std;

bool
is_prime(int x)
{
    for (int i = 2; i < x; ++i)
        if (x % i == 0)
            return false;
    return true;
}

int
tst_000()
{
    // call function asynchronously:
    std::future < bool > fut = std::async(is_prime, 444444443);

    // do something while waiting for function to set future:
    std::cout << "checking, please wait";
    std::chrono::milliseconds span(100);
    while (fut.wait_for(span) == std::future_status::timeout)
        std::cout << '.';

    bool x = fut.get();         // retrieve return value

    std::cout << "\n444444443 " << (x ? "is" : "is not") << " prime.\n";

    return 0;
}

int do_get_value()
{
    return 10;
}
int tst_001()
{
    std::future<int> fut = std::async(do_get_value);
    std::shared_future<int> shared_fut = fut.share();

    std::cout << "value: " << shared_fut.get() << std::endl;
    std::cout << "value * 2: " << shared_fut.get() << std::endl;

    return 0;
}

void get_int(std::promise<int>& prom)
{
    int x;
    std::cout << "Please enter an integer value: ";
    std::cin.exceptions(std::ios::failbit);

    try {
        std::cin >> x;
        prom.set_value(x);
    } catch (std::exception&) {
        prom.set_exception(std::current_exception());
    }
}

void print_int(std::future<int>& fut)
{
    try {
        int value = fut.get();
        std::cout << "value: " << value << std::endl;
    } catch (std::exception &e) {
        std::cout << "[caught exception: " << e.what() << "]" << std::endl;
    }
}

int tst_002()
{
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread th1(get_int, std::ref(prom));
    std::thread th2(print_int, std::ref(fut));

    th1.join();
    th2.join();

    return 0;
}

int tst_003()
{
    std::future<int> foo, bar;      // future 初始化，使用默认构造函数，均为invalid。
    foo = std::async(do_get_value); // async 使用move语义赋值
    bar = std::move(foo);           // 此时 foo 是invalid

    if(foo.valid())
        std::cout << "foo's value: " << foo.get() << std::endl;
    else
        std::cout << "foo is invalid." << std::endl;

    if(bar.valid())
        std::cout << "bar's vallue: " << bar.get() << std::endl;
    else
        std::cout << "bar is invalid." << std::endl;

    return 0;
}

int tst_004()
{
    std::future<bool> fut = std::async(is_prime, 19423241);

    std::cout << "Checking... \n";
    fut.wait();

    std::cout << "19423241 ";
    if(fut.get())
        std::cout << "is prime.\n";
    else
        std::cout << "is not prime.\n";

    return 0;
}

int tst_005()
{
    std::future<bool> fut = std::async(is_prime, 194232491);

    std::cout << "Checking...";
    std::chrono::milliseconds span(20);

    while(fut.wait_for(span) == std::future_status::timeout) {
        cout << ".";
        cout.flush();
    }

    std::cout << "\n 194232491 ";
    if(fut.get())
        std::cout << "is a prime.\n";
    else
        std::cout << "is not a prime.\n";

    return 0;
}

double ThreadTask(int n)
{
    std::cout << "Task 0x" << std::ios::hex << std::this_thread::get_id() << " is computing...\n";

    double ret = 0;
    for (int i = 0; i < n; ++i) {
        ret += std::sin(i);
    }

    std::cout << "Task 0x" << std::ios::hex << std::this_thread::get_id() << " computing finished.\n";
    return ret;
}

int tst_006()
{
    std::future<double> f(std::async(std::launch::async, ThreadTask, 100000));

#if 0
    while(f.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1)) != std::future_status::ready) {
        std::cout << "Task is runing... \n";
    }
#else
    while(f.wait_for(std::chrono::seconds(1)) != std::future_status::ready) {
        std::cout << "Task is runing... \n";
    }
#endif

    std::cout << f.get() << std::endl;

    return 0;
}

void do_print_ten_times(char c, int ms)
{
    for(int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::microseconds(ms));
        std::cout << c;
    }
}

int tst_007()
{
    std::cout << "with launch async.\n";
    std::future<void> foo(std::async(std::launch::async, do_print_ten_times, '*', 100));
    std::future<void> bar(std::async(std::launch::async, do_print_ten_times, '@', 200));
    foo.get();
    bar.get();
    std::cout << "\n\n";

    std::cout << "with launch deferred.\n";
    foo = std::async(std::launch::deferred, do_print_ten_times, '*', 100);
    bar = std::async(std::launch::deferred, do_print_ten_times, '@', 200);
    foo.get();
    bar.get();
    std::cout << "\n\n";

    return 0;
}

int main()
{
    tst_007();
    return 0;
}
