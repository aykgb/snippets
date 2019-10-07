#include <iostream>
#include <vector>

#include <string.h>
#include <stdio.h>

using namespace std;

class Base {
public:
    Base()
    {
        cout << "It's Base's constructor!" << endl;
        index = 10;
    }
    ~Base()
    {
        cout << "It's Base's destructor!" << endl;
    }

    void getIndex() const
    {
        cout << "It's the Base's getIndex() function." << endl;
        cout << index << endl;
    }

private:
    int index;
//	static int index;
};

int main()
{
    Base baseTst;
    Base *ptrBase;

    char memtmp[sizeof(baseTst)];
    memcpy(memtmp, &baseTst, sizeof(baseTst));
    printf("=============\n");
    printf("%d\n",static_cast<int>(sizeof(baseTst)));
    printf(memtmp);
    printf("=============\n");

    ptrBase = reinterpret_cast<Base *>(memtmp);
    ptrBase->getIndex();

    return 0;
}
