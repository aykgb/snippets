#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>

#include <stdio.h>
#include <string.h>

using namespace std;

class BufStream {
    char delim = '\n';

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
            return 0;
        }

        char *pfind = strchr(pcur, delim);
        if (pfind == nullptr ) {
            if(size_already_read < size_total) {
                output.assign(pcur, size_total - size_already_read);
                size_already_read = size_total;
                return 11; // end of buf with non '\n'
            }
        }
        int len = pfind - pcur;

        /* count character '\n' */
        size_already_read += len + 1;
        if (size_already_read >= size_total) {
            size_t size_last = size_total - (size_already_read - len - 1);
            if (pstr[size_total - 1] == '\n') {
                size_last--;
            }
            output.assign(pcur, size_last);
            if (size_already_read > size_total) {
                size_already_read = size_total;
                return 12; // end of buf with non '\n' and strchr had been out of buf bounds
            }
            return 1; // end of buf with '\n'
        }
        output.assign(pcur, len);

        pcur = pfind + 1;

        return 1; // find a line with delimeter '\n'
    }

    void reset() {
        pcur = pstr;
    }
};


int TEST()
{
    const size_t BUF_SIZE = 1024 * 256;
    char* buf = new char[BUF_SIZE];
    FILE* fp = fopen("gizzle_queue_neighbor", "rw+");

    auto beg = chrono::system_clock().now();
    auto end = chrono::system_clock().now();

    size_t line_counter = 0;
    string line_incomplete;
    std::ofstream ofs("gizzle_queue_neighbor.bk");
    for(int i = 0; i < 1000000; i++) {
        auto ret = fread(buf, 1, BUF_SIZE, fp);
        // string strbuf(buf, ret);
        // istringstream iss(strbuf);

        BufStream bs(buf, ret);

        string line;
        // while(std::getline(iss, data)) {
        //      std::cout << data << "\n";
        // }

        while(true) {
            int ret = bs.getLineFromCharArr(line);
            if (ret == 0) {
                break;
            } else {
                if (ret != 1) {
                    line_incomplete = line;
                    // std::cout << "<== incomplete line " << line_incomplete << std::endl;
                    continue;
                }
            }
            if(!line_incomplete.empty()) {
                line_incomplete.append(line);
                line = std::move(line_incomplete);
                if(!line_incomplete.empty()) {
                    std::cout << "<==> err\n";
                }
                // std::cout << "==> complete line " << line << std::endl;
            }
            line_counter++;
            ofs << line << "\n";
        }

        if (ret < BUF_SIZE) {
            std::cout << "total line of file " << line_counter << "\n";
            break;
        }

        // std::cout << strbuf << std::endl;
        // buf[ret] = '\0';
        // printf("%s\n", buf);
    }

    end = chrono::system_clock().now();

    std::cout << chrono::duration_cast<chrono::milliseconds>(end - beg).count() << " ms" << std::endl;

    delete[] buf;

    return 0;
}
