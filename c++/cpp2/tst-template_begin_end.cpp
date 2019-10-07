#include <iostream>

template<size_t M, typename T>
T* begin(T (&p)[M])   // can't return a const type parameter.
{
    return &p[0];
}

template<size_t M, typename T>
T* end(T (&p)[M])
{
    return &p[M - 1];
}

int main()
{
    int foo[] = {10, 20, 30, 40, 50};

    for(auto it = begin(foo); it != end(foo); ++it) {
        std::cout << *it << std::endl;
    }
}
