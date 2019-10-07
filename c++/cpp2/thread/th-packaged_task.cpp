#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <utility>

using namespace std;

int countdown(int from, int to)
{
    for (int i = from; i != to; --i) {
        std::cout << i << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Finished!\n";

    return from - to;
}

int tst000()
{
    std::packaged_task<int(int, int)> task(countdown);
    std::future<int> ret = task.get_future();

    std::thread th(std::move(task), 10, 0);
    // std::thread th(task, 10, 0); // the copy constructor of packaged_task has been deleted. So this line shall not compile pass.
    int value = ret.get();

    std::cout << "The countdown lasted for " << value << " seconds.\n";

    th.join();
    return 0;
}

int tst001()
{
    std::packaged_task<int(int)> foo;

    std::packaged_task<int(int)> bar([](int x) -> int { return x*2; });

    foo = std::move(bar);

    std::future<int> ret = foo.get_future();

    std::thread(std::move(foo), 10).detach();

    int value = ret.get(); // wait for mission complete and get the result.

    std::cout << "The double of 10 is " << value << ".\n";

    return 0;
}

std::future<int> launcher(std::packaged_task<int(int)>& tsk, int arg)
{
    if(tsk.valid()) {
        std::future<int> ret = tsk.get_future();
        std::thread (std::move(tsk), arg).detach();
        return ret;
    } else
        return std::future<int>();
}

int tst002()
{
    std::packaged_task<int(int)> tsk([] (int x) {
        return x*2;
    });

    std::future<int> fut = launcher(tsk, 25);

    std::cout << "The double of 25 is " << fut.get() << ".\n";

    return 0;
}

int tst003()
{
    std::packaged_task<int(int)> tsk([](int x) {
        return x * 3;
    });

    std::future<int> fut = tsk.get_future();

    std::thread(std::move(tsk), 100).detach();

    int value = fut.get();

    std::cout << "the tripple of 100 is " << value << ".\n";

    return 0;
}

int triple (int x)
{
    return x * 3;
}

int tst004()
{
    std::packaged_task<int(int)> tsk(triple); // package task

    std::future<int> fut = tsk.get_future();

    // std::thread(std::move(tsk), 100).detach(); // Using the std::move() will causecode at line 107 running error.
    std::thread(std::ref(tsk), 100).detach();

    std::cout << "The triple of 100 is " << fut.get() << ".\n";

    // re-use same task object:
    tsk.reset();
    fut = tsk.get_future();
    std::thread(std::move(tsk), 200).detach();
    std::cout << "The triple of 200 is " << fut.get() << ".\n";

    return 0;
}

int main()
{
    // tst000();

    // tst001();

    // tst002();

    // tst003();


    tst004();

    return 0;
}
