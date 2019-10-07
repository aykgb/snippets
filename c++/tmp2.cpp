#include <iostream>
#include <vector>
#include <set>
#include <string.h>

using namespace std;

class Test {
public:
    Test();
    ~Test();

    void assignMyData();
    void printMyData();

private:
    vector<set<int> > iVec;
    static const int slotIds[];
};

const int Test::slotIds[] = {1, 2, 3, 4, 5};

void Test::assignMyData()
{
    for(int i = 0; i < 20; ++i) {
        iVec.push_back(set<int>());
        for(int j = 0; j < 20000; ++j) {
            iVec[i].insert(j);
        }
    }
    return;
}

void Test::printMyData()
{
    for(int i = 0; i < 20; ++i) {
        printf("0x%p\n", &iVec[i]);
    }
    return;
}

Test::Test(void)
{}

Test::~Test(void)
{
    for(vector<set<int> >::iterator itr = iVec.begin(); itr != iVec.end(); ++itr)
        (*itr).clear();
    iVec.clear();

    return;
}

class NoStackObject {

protected:

    ~NoStackObject() { }

public:

    void destroy()
    {
        delete this ;//调用保护析构函数
    }

};

int main()
{
    Test* myTest = new Test();

    myTest->assignMyData();
    myTest->printMyData();

    vector<int> vec;
    vec.push_back(0);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    vec.clear();

    for(int i = 0; i < 4; ++i) {
        printf("vec[%d] = %d\n", i, vec[i]);
    }

    // delete myTest;

    // NoStackObject obj;

    cout << vec[4] << endl;

    return 0;
}