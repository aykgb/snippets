#include <iostream>
#include <thread>
#include <atomic>

std::atomic <int> foo(0);

void set_foo(int x)
{
    // foo = x;
    foo.store(x, std::memory_order_relaxed);
}

void print_foo()
{
    // while(foo == 0) {
    //   std::this_thread::yield();
    // }
    // std::cout << "foo: " << foo << "\n";
    int x;
    do {
        x = foo.load(std::memory_order_relaxed);
    } while (x == 0);
    std::cout << "foo: " << foo << "\n";
}

int tst_000()
{
    std::thread first(print_foo);
    std::thread second(set_foo, 10);

    first.join();
    second.join();

    return 0;
}

int main()
{
    tst_000();
    return 0;
}
