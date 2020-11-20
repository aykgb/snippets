#include "lm.helloworld.pb.h"
#include <fstream>
#include <iostream>

using namespace std;

void ListMsg(const lm::helloworld &msg)
{
    cout << msg.id() << "\n";
    cout << msg.str() << "\n";
}

int reader_main()
{
    lm::helloworld msg;
    {
        fstream input("./log", ios::in | ios::binary);
        if(!msg.ParseFromIstream(&input)) {
            cerr << "Failed to parse address book." << endl;
            return -1;
        }
    }

    ListMsg(msg);

    return 0;
}
