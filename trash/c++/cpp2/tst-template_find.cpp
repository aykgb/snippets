#include <iostream>
#include <string>
#include <vector>
#include <list>

template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T& val)
{
    while(first != last) {
        if(*first==val) return first;
        ++first;
    }
    return last;
}

using namespace std;

int main()
{
    vector<int> t0{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    list<string> t1{"hello", "template", "This", "is", "a", "test", "case"};

    vector<int>::iterator it0 = find(t0.begin(), t0.end(), 6);
    cout << *it0 << endl;

    list<string>::iterator it1 = find(t1.begin(), t1.end(), "This");
    cout << *it1 << endl;

    return 0;

}
