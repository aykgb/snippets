#include <iostream>

template<typename T, size_t M>
constexpr size_t array_size(const T(&)[M])
{
    return M;
}

int main()
{
    int k[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << array_size(k) << std::endl;

    return 0;
}
