#include <iostream>
#include <functional>
#include <thread>
#include <future>

std::promise<int> prom;

void print_int(std::future<int>& fut)
{
    try {
        int x = fut.get() ;  // Get the shared the value.
        std::cout << "value: " << x << '\n'; // Print the value.
    } catch (std::exception& e) {
        std::cout << "[exception caught: " << e.what() << "]\n";
    }

}

void print_global_promise()
{
    std::future<int> fut = prom.get_future();
    int x = fut.get();
    std::cout << "value: " << x << '\n';
}

void get_int(std::promise<int>& prom)
{
    int x;
    std::cout << "Please, enter an integer value: ";
    std::cin.exceptions (std::ios::failbit); // throw on failbit
    try {
        std::cin >> x;                         // sets failbit if input is not int
        prom.set_value(x);
    } catch (std::exception&) {
        prom.set_exception(std::current_exception());
    }
}

int tst000()
{
    // Generator a std::promise<int> object.
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread t(print_int, std::ref(fut));
    prom.set_value(10); // Setting the shared statr, syncronize with thread t.

    t.join();
    return 0;
}

int tst001()
{
    std::thread th1(print_global_promise);
    prom.set_value(10);
    th1.join();

    prom = std::promise<int>();

    std::thread th2 (print_global_promise);
    prom.set_value(20);
    th2.join();

    return 0;
}

int tst002()
{
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread th1(get_int, std::ref(prom));
    std::thread th2(print_int, std::ref(fut));

    th1.join();
    th2.join();

    return 0;
}

int main()
{
    // tst000();
    // tst001();
    tst002();

    return 0;
}
