#include <iostream>
#include <memory>

using namespace std;

int main() {
    std::shared_ptr<int> pi(new int(1));

    std::cout << *pi << std::endl;

}
