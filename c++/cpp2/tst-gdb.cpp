#include <iostream>

int val = 0;

void dummy_function()
{
    val += 2;
    std::cout << val << std::endl;
}

int main(int argc, char **argv)
{
    for(int i = 0; i < 50; ++i) {
        dummy_function();
    }

    return 0;
}
