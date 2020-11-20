#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

using namespace std;

class HadoopCat {
    string hdfs_path;
    bool is_dir;

    FILE *pp = nullptr;

public:
    HadoopCat(string path): hdfs_path(path), is_dir(false) { }

    int Init() {
        string cmd ="hadoop fs -cat " + hdfs_path;
        pp = popen(cmd.c_str(), "r");
        if (!pp) {
            printf("popen error :%d\n", errno);
            return -1;
        }
        return 0;
    }

    int getStreamFromHadoop(char *output, int output_length) {
        char *ret = fgets(output, output_length, pp);
        if(feof(pp)) {
            printf("reach the end of file %s.\n", hdfs_path.c_str());
            return 0;
        } else {
            return -1;
        }
        return output_length;
    }
};

int TEST() {
    HadoopCat hadc("/user/strategy_04/rocks_table/gesake_value_model.gz");
    ofstream ofs("ouput.gz");

    while(true) {
        if(hadc.Init()) {
            printf("failed to Init.\n");
            return -1;
        }

        char output[1024];
        int read_size = hadc.getStreamFromHadoop(output, 1024);
        if (read_size <= 0) {
            if (read_size) {
                printf("failed to getStreamFromHadoop\n");
                return -1;
            }
            printf("finished.\n");
            return 0;
       }
       std::string res(output, read_size);
       ofs << res;
    }
}

int TEST01() {
    string res;

    FILE *pp = popen("hadoop fs -cat /user/strategy_04/rocks_table/gesake_value_model.gz", "r");
    if (!pp) {
        printf("popen error :%d\n", errno);
        res = "fork or pipe fail";
        return -1;
    }

    std::cout << "step 0\n";
    char tmp[1024]; //设置一个合适的长度，以存储每一行输出
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        // if (tmp[strlen(tmp) - 1] == '\n') {
        //     tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        // }
        printf("%s", tmp);
        std::cout << "step 01\n";
    }
    std::cout << "step 1\n";

    pclose(pp);

    return 0;
}
