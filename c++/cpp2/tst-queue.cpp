#include <iostream>
#include <queue>

using namespace std;
using namespace std;

int main()
{
    std::queue<int> task;

    try {
        std::cout << task.size() << std::endl;
        task.pop();
        std::cout << task.size() << std::endl;
        task.push(3);
        std::cout << task.size() << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
