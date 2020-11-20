#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

bool my_popen (const std::string& cmd,std::vector<std::string>& out )
{
    FILE*           fp;
    const int       SIZEBUF = 1234;
    char            buf [SIZEBUF];
    out = std::vector<std::string> ();
    if ((fp = popen(cmd.c_str (), "r")) == NULL) {
        return false;

    }
    std::string  cur_string = "";
    while (fgets(buf, sizeof (buf), fp)) {
        cur_string += buf;
    }
    out.push_back (cur_string.substr (0, cur_string.size () - 1));
    pclose(fp);
    return true;
}

int tst000()
{
    std::vector<std::string> output;
    my_popen("ls -l > /dev/null ", output);
    for ( std::vector<std::string>::iterator itr = output.begin();
            itr != output.end();
            ++itr) {
        std::cout << *itr << std::endl;

    }

    return 0;
}

int tst001()
{
    /* Core code */
    std::ostringstream oss;
    std::streambuf *sysbuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    FILE *fp = popen("ls -lrt", "r");
    if(fp == NULL) {
        return -1;
    }

    char line[1024];
    while(fgets(line, 1024, fp) != NULL) {
        cout << line;
    }
    pclose(fp);
    std::cout.rdbuf(sysbuf);
    /* Ends */

    std::cout << oss.str();

    return 0;
}

int main()
{
    tst001();

    return 0;
}
