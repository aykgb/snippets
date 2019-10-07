#include <iostream>
#include <vector>

using namespace std;

struct Hop {
    Hop():x(0), y(0), d(0) { }
    unsigned short x;
    unsigned short y;
    unsigned short d; // direction
};

std::vector<Hop> find_next_possible_hop(Hop cur)
{
    // The current Hop value must be 1.
    Hop pos[4] = {cur, cur, cur, cur};
    pos[0].y = cur.y + 1;
    pos[1].x = cur.x + 1;
    pos[2].y = cur.y - 1;
    pos[3].x = cur.x - 1;
}

int main()
{
    unsigned short m, n, P;
    std::cin >> m >> n >> P;
    if(!(m >= 3 && n <= 10 && P >= 1 && P <= 10))
        std::cout << "err: Input invalid.\n";    // 检查输入合法性

    char *matrix = new char[n * m];
    for(int i = 0; i < m * n; ++i) {
        std::cin >> matrix[i];
    }

    // 检查极端条件
    if(m > 101) std::cout << "Can not escape!\n";

    // 算法实现


    delete matrix;
    return 0;
}
