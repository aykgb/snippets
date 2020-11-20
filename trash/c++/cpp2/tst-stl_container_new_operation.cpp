/*************************************************************************
    > File Name: tst-stl_container_new_operation.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Fri 22 Apr 2016 09:41:57 AM CST
 ************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

class Test {
public:
    Test(const uint32_t ii): i(ii)
    {
        cout << "Test()\n";
    }
    ~Test()
    {
        cout << "~Test()\n";
    }

    void print()
    {
        cout << i << "\n";
    }

private:
    int i;
};

int main()
{
    vector<Test *> cvec;

    for(int i = 0; i < 3; ++i) {
        cvec.push_back(new Test(i));
    }

    vector<Test *>::iterator  it = cvec.begin();
    for(int i = 0; i < 3; ++i) {
        //            delete cvec[i];
        cvec.erase(it++);
    }

    return 0;
}
