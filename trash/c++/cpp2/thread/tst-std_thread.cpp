#include <thread>
#include <iostream>
#include <functional>
#include <mutex>

using namespace std;

int main()
{
    cout << thread::hardware_concurrency() << endl;

    std::thread t([]() {
        std::cout << "Hello thread!\n";
    });

    std::function<void(std::thread t)> by_lvalue = [](std::thread) {};
    std::function<void(std::thread& t)> by_lvalue_ref = [](std::thread&) {};
    std::function<void(std::thread&& t)> by_rvalue_ref = [](std::thread&&) {};

    //  std::thread copy = t;
    std::thread t2 = std::move(t);

    //  by_lvalue(t2);
    //  by_lvalue_ref(t2);
    //  by_rvalue_ref(std::move(t2));

    //  auto tt = std::thread{[]() {
    //  throw "raw exception";
    //  }};

    //  tt.join();

    t.join();
    t2.join();

    if(t.joinable()) {
        t.join();
    } else {
        cout << "This thread is not joinable now.\n";
    }

    return 0;
}
