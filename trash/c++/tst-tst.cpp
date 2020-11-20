#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

int main()
{
    char cont;
    cin >> cont;

    vector<int> slotIds;
    const int ids[] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 16, 1, 2, 3, 4, 7, 8, 9, 10};
    printf("%d\n", int(sizeof(ids)/sizeof(int)));
    return 0;
}