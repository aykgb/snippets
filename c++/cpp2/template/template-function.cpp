#include <iostream>
#include <cstring>

using namespace std;

template<typename T>
int compare(const T& v1, const T& v2)
{
    if (v1 < v2) return -1;
    if (v2 < v1) return 1;
    return 0;
}

template<unsigned M, unsigned N>
int compare(const char (&p1)[M], const char (&p2)[N])
{
    return strcmp(p1, p2); // Shall compare the first character, and then return
    // the distance between the two head-character.
}

int main()
{
    std::cout << compare(1, 0) << std::endl;
    std::cout << compare("she", "he") << std::endl;
    return 0;
}
