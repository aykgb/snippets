#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>

using namespace std;

int main()
{
    ofstream ofile;
    unsigned int port_id = 0;
    std::bitset<8> reason_mask("00001111");
    bool als_enable = true;
    bool laser_shut = false;
    ofile.open("tst.txt");
    ofile << "This a test line.\n";
    ofile << port_id << " ";
    ofile << reason_mask << " " ;
    ofile << als_enable << " ";
    ofile << laser_shut << "\n";
    ofile.close();

    ifstream ifile;
    string line;
    ifile.open("tst.txt");
    //  ifile >> std::cout;
    std::getline(ifile, line);
    std::getline(ifile, line);
    stringstream ss;
    ss << line;
    // std::cout << line << "\n";
    unsigned int a;
    std::bitset<8> b;
    bool c;
    bool d;
    ss >> a;
    ss >> b;
    ss >> c;
    ss >> d;
    std::cout << a << " " << b << " " << c << " " << d << "\n";
    ifile.close();

    return 0;
}
