#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#include <string.h>

using namespace std;

class BufStream {
    char delm = '\n';

    char *pstr = nullptr;
    char *pcur = nullptr;
    size_t size_total = 0;
    size_t size_already_read = 0;
  public:
    BufStream(char *src, size_t len): pstr(src), size_total(len) {
        pcur = src;
    }

    int getLineFromCharArr(string& output) {
        if(size_already_read >= size_total) {
            output.clear();
            return 0;
        }

        char *pfind = strchr(pcur, delm);
        if (pfind == nullptr ) {
            if(size_already_read < size_total) {
                printf("size_total %ld size_alread_read %ld\n", size_total, size_already_read);
                output.assign(pcur, size_total - size_already_read);
                return size_total - size_already_read;
            }
        }
        int len = pfind - pcur;

        /* 1 represent character '\n' */
        size_already_read += len + 1;
        if (size_already_read >= size_total) {
            size_t size_last = size_total - (size_already_read - len - 1);
            output.assign(pcur, size_last);
            size_already_read = size_total;
            return size_last;
        }
        output.assign(pcur, len);

        pcur = pfind + 1;

        return len;
    }

    void reset() {
        pcur = pstr;
    }
};


int main() {
    char buf[] = {'1', 'a', 'b', '\n', '2', '\n', 'e', '\0'};
    BufStream bs(buf, 2);
    string data("initialization");

    int len = bs.getLineFromCharArr(data);
    std::cout << "len " << len << " |str " << data << " |str.size " << data.size() << "\n";
    len = bs.getLineFromCharArr(data);
    std::cout << "len " << len << " |str " << data << " |str.size " << data.size() << "\n";
    len = bs.getLineFromCharArr(data);
    std::cout << "len " << len << " |str " << data << " |str.size " << data.size() << "\n";

    return 0;
}
