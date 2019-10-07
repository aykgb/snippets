/*************************************************************************
    > File Name: tst-insert_emplace.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Thu 21 Apr 2016 11:04:50 AM CST
 ************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

class Test {
public:
    Test(int ii): i(ii)
    {
        cout << "#### Constructor. this->i:" << this->i  << "\n";
    }
    Test(const Test &obj):i(obj.i)
    {
        cout << "#### Copy constructor. this->i:" << this->i << "\n";
    }
    ~Test()
    {
        cout << "#### Destructor. this->i:" << this->i << " \n";
    }

    int get_i()
    {
        return i;
    }

    void set_i(int ii)
    {
        i = ii;
    }

private:
    int i;
};

int main()
{
    vector<Test> tvec;
    tvec.push_back(0);
    tvec.emplace_back(1);

    return 0;
}

