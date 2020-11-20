#include <future>
#include <iostream>

using namespace std;

int task(std::promise<int>&& p) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    p.set_value(1);
    return 0;
}

int main() {
    std::promise<int> promise_fut;
    auto fut = promise_fut.get_future();

    std::thread woker(task, std::move(promise_fut));

    fut.wait();

    woker.join();

    std::cout << fut.get() << std::endl;

    return 0;
}
