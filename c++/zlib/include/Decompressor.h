#pragma once

#include "zlib.h"

#include <assert.h>

#include <string>

// The windowBits parameter is the base two logarithm of the window size (the size of the history buffer).
// It should be in the range 8..15 for this version of the library.
// Larger values of this parameter result in better compression at the expense of memory usage.
// This range of values also changes the decoding type:
//  -8 to -15 for raw deflate
//  8 to 15 for zlib
// (8 to 15) + 16 for gzip
// (8 to 15) + 32 to automatically detect gzip/zlib header
constexpr int WINDOW_BITS_ZLIB = 15;
constexpr int WINDOW_BITS_GZIP = WINDOW_BITS_ZLIB + 16;
constexpr int WINDOW_BITS_AUTO = WINDOW_BITS_ZLIB + 32;

constexpr int CHUNK = 1024 * 256; // 256K
constexpr int INFLATE_FACTOR = 4; // 256K x 4= 1M

// 使用zlib解压一个文件gz
class Decompressor {
    const std::string gzfileName;
    FILE* source = nullptr;
    size_t file_size = 0;
    size_t decompressd_size = 0;

    z_stream strm;
    unsigned char in[CHUNK];

    int chunk_read_counter = 0;

    gzFile gzfp = nullptr;

    bool init_gzread = false;
public:
    Decompressor(const std::string& fn): gzfileName(fn) { }

    ~Decompressor() {
        if(!init_gzread) {
            if(source != nullptr) {
                fclose(source);
            }
            (void)inflateEnd(&strm);
        } else {
            if (gzfp != nullptr) {
                gzclose(gzfp);
            }
        }
    }

    // 初始化
    int Init();

    int InitGZRead();

    // 参考zlib example
    int GZUncompressToFile(std::string outfile = "");

    int GZUncompressToFileByGZRead(std::string outfile = "");

    // 用户负责output_data的new和delete工作
    int GZUncompress(unsigned char * output_data, size_t output_length, int *decompress_size);

    int GZUncompressByGZRead(char * output_data, size_t output_length);

    // using rocksdb implementation.
    // 这段代码必须输入完整的gz数据，否者会报Z_BUF_ERROR
    char* Zlib_Uncompress(const char* input_data, size_t input_length,
                             int* decompress_size,
                             int windowBits = WINDOW_BITS_AUTO);

    size_t getGZFileSize() {
        return file_size;
    }

    size_t getDecompressedSize() {
        return decompressd_size;
    }
};

