#include <iostream>
#include <future>
#include <chrono>
#include <stdio.h>

// a non-optimized way of checking for prime numbers:
bool
is_prime(int x)
{
    for (int i = 2; i < x; ++i)
        if(x % i == 0)
            return false;
    return true;
}

int
tst000()
{
    // call function asynchronously
    std::future<bool> fut = std::async(is_prime, 27884012);

    // do something while waiting for function to set future;
    std::cout << "Checking, please wait..." << std::endl;
    std::chrono::milliseconds span(1000);
    while (fut.wait_for(span) == std::future_status::timeout)
        printf(".");

    bool x = fut.get(); // retrieve return value

    std::cout << "\n23787384012 " << (x ? "is" : "is not") << " prime.\n";

    return 0;
}

int
main()
{
    tst000();

    return 0;
}
