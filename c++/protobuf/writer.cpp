/*************************************************************************
    > File Name: writer.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Wed 20 Apr 2016 07:45:46 PM CST
 ************************************************************************/

#include<iostream>
#include<fstream>
#include"lm.helloworld.pb.h"

using namespace std;

int writer_main()
{
    lm::helloworld msg;
    msg.set_id(101);
    msg.set_str("hello");

    /* Write the new address book back to disk. */
    fstream output("./log", ios::out | ios::trunc | ios::binary);
    if(!msg.SerializeToOstream(&output)) {
        cerr << "Failed to write msg.\n";
        return -1;
    }

    return 0;
}

