#include <iostream>
#include <vector>

#include <string.h>
#include <stdio.h>

using namespace std;

class Base
{
public:
	Base() { cout << "It's Base's constructor!" << endl; }
	~Base() { cout << "It's Base's destructor!" << endl; }
	
	void fun()
	{
		cout << "It's the Base's fun function." << endl;
	}
	
private:
	int index;
	static int index;
};

int main()
{
	Base baseTst;
	
	char memtmp[sizeof(baseTst)];
	memcpy(memtmp, &baseTst, sizeof(baseTst));
	
	return 0;
}