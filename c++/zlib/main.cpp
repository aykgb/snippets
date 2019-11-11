#include <iostream>

#include <stdio.h>

#include "TEST.hpp"
#include "include/Decompressor.h"
#include "include/HadoopCat.hpp"

using namespace std;

int main(int argc, char **args) {
    FILE *infd = NULL;
    FILE *outfd = NULL;

    std::string cmd;
    if (argc > 1) {
        cmd = std::string(args[1]);
    }

    if (cmd == "infgz") {
        if (argc < 4) {
            std::cout << "Usage: ./main infgz src.gz dst\n";
            return 0;
        } else {
            char *src = args[2];
            char *dst = args[3];
            infd = fopen(src, "r");
            outfd = fopen(dst, "w+");
            printf("%s\n", src);
            printf("%s\n", dst);
            gzip::infgz(infd, outfd);
        }
    } else if (cmd == "infgz_no_output") {
        if (argc < 3) {
            std::cout << "Usage: ./main infgz_no_output src.gz\n";
            return 0;
        } else {
            char *src = args[2];
            infd = fopen(src, "r");
            printf("%s\n", src);
            gzip::infgz_no_output(infd);
        }
    } else if (cmd == "infgz_use_gzf") {
        if (argc < 3) {
            std::cout << "Usage: ./main infgz_use_gzf src.gz\n";
            return 0;
        } else {
            printf("%s\n", args[2]);
            gzip::infgz_use_gzf(args[2]);
        }
    } else if (cmd == "decomp") {
        if (argc < 3) {
            std::cout << "Usage: ./main decomp src.gz\n";
            return 0;
        }
        char *src = args[2];
        infd = fopen(src, "r");
        if (infd == NULL) {
            printf("error open file %s\n", src);
            return -1;
        }

        printf("%s\n", src);

        constexpr int CHUNK = 1024 * 16;
        char input_data[CHUNK];

        Decompressor decomp(src);
        while(true) {
            int input_length = fread(input_data, 1, CHUNK, infd);
            if (ferror(infd)) {
                std::cout << "fread error." << std::endl;
                break;
            }
            if (input_length == 0) {
                std::cout << "fread finished." << std::endl;
                break;
            }

            int decompress_size = 0;
            char * output_data = decomp.Zlib_Uncompress(input_data, input_length, &decompress_size);
            if (output_data == nullptr || decompress_size == 0) {
                std::cout << "decompress failed." << std::endl;
                break;
            }

            delete output_data;
        }

        }
    else if (cmd == "decomp2f") {
        if (argc < 3) {
            std::cout << "Usage: ./main decomp2f src.gz\n";
            return 0;
        }
        char *src = args[2];
        printf("%s\n", src);

        Decompressor decomp(src);
        if (decomp.Init() == -1) {
            std::cout << "open .gz file error.\n";
            return 0;
        }

        int ret = decomp.GZUncompressToFile();
        if ( ret != Z_OK) {
            std::cout << "failed to decompress a .gz file. ret " << ret << std::endl;
            return 0;
        }
    } else if(cmd == "gzread2f") {
        if (argc < 3) {
            std::cout << "Usage: ./main gzread2f src.gz\n";
            return 0;
        }
        char *src = args[2];
        printf("%s\n", src);

        Decompressor decomp(src);
        if (decomp.InitGZRead() < 0) {
            std::cout << "failed to init a .gz file." << std::endl;
            return 0;
        }
        int ret = decomp.GZUncompressToFileByGZRead();
        if ( ret != Z_OK) {
            std::cout << "failed to decompress a .gz file. ret " << ret << std::endl;
            return 0;
        }
    } else if(cmd == "hadoopcat") {
        if (argc < 3) {
            std::cout << "Usage: ./main hadoopcat hdfs_path\n";
            return 0;
        }
        char *src = args[2];
        printf("%s\n", src);

        HadoopCat hadc(src);
        if(hadc.Init()) {
            printf("failed to Init.\n");
            return -1;
        }
        ofstream ofs("ouput.gz");

        while(true) {
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
    } else {
        std::cout << "Usage:\n"
                  << "    infgz src.gz dst\n"
                  << "    infgz_no_output src.gz\n"
                  << "    infgz_use_gzf src.gz\n"
                  << "    decomp src.gz\n"
                  << "    decomp2f src.gz\n"
                  << "    gzread2f src.gz\n"
                  << "    hadoopcat hdfs_path\n"
                  << std::endl;
    }

    return 0;
}