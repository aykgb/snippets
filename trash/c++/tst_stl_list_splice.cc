#include <iostream>
#include <iterator>
#include <list>

using namespace std;

void print_list(std::list<int>& l) {
    for (auto iter = l.begin() ; iter != l.end(); iter++) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::list<int> li;

    li.push_back(1);
    li.push_back(2);
    li.push_back(3);
    li.push_back(4);
    li.push_back(5);
    print_list(li);

    auto iter = std::next(li.begin(), 2);
    li.splice(li.begin(), li, iter);
    print_list(li);

    iter = std::next(li.begin(), 3);
    li.splice(li.end(), li, iter);
    print_list(li);
}