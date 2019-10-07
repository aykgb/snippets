#include <iostream>

template<unsigned M, typename T>
void print(const T (&p)[M])
{
    for(auto elem : p)
        std::cout << elem << std::endl;
}

int main()
{
    int k[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    print(k);

    return 0;
}
