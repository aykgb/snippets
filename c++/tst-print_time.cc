#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

#include <time.h>

using namespace std;

int premakestr(char* m_buffer) {
    time_t now;
    now = time(&now);;
    struct tm vtm;
    localtime_r(&now, &vtm);

    return snprintf(m_buffer, 20, "%04d-%02d-%02d %02d:%02d:%02d",
                    vtm.tm_year + 1900,
                    vtm.tm_mon + 1,
                    vtm.tm_mday,
                    vtm.tm_hour,
                    vtm.tm_min,
                    vtm.tm_sec);
}

int main() {
    char buffer[20];
    int ret = premakestr(buffer);

    std::string timestamp(buffer);

    std::cout << "||timestamp=" + timestamp + "||" << std::endl;
    std::cout << "ret=" << ret << std::endl;

    return 0;
}
