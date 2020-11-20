#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

void run()
{
    std::cout << "This thread id: " << std::this_thread::get_id() << std::endl;
    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

std::thread t;

int start()
{

    t = std::thread(run);

    return 0;

}

int main()
{
    start();

    std::cout << "main thread...\n";

    if(t.joinable())
        t.join();
    else
        std::cout << "not joinable.\n";

    return 0;
}
