#include "include/Decompressor.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <algorithm>
#include <iostream>

using namespace std;

// using rocksdb implementation
char* Decompressor::Zlib_Uncompress(const char* input_data, size_t input_length,
                                   int* decompress_size,
                                   int windowBits) {
    uint32_t output_len = 0;
    // Assume the decompressed data size will 5x of compressed size, but round
    // to the page size
    size_t proposed_output_len = ((input_length * 5) & (~(4096 - 1))) + 4096;
    output_len = static_cast<uint32_t>(
                     std::min(proposed_output_len,
                              static_cast<size_t>(std::numeric_limits<uint32_t>::max())));

    z_stream _stream;
    memset(&_stream, 0, sizeof(z_stream));

    int st = inflateInit2(&_stream, windowBits);
    if (st != Z_OK) {
        return nullptr;
    }

    _stream.next_in = (Bytef *)input_data;
    _stream.avail_in = static_cast<unsigned int>(input_length);

    char* output = new char[output_len];

    _stream.next_out = (Bytef *)output;
    _stream.avail_out = static_cast<unsigned int>(output_len);

    bool done = false;
    while (!done) {
        st = inflate(&_stream, Z_SYNC_FLUSH);
        switch (st) {
        case Z_STREAM_END:
            done = true;
            break;
        case Z_OK: {
            // No output space. Increase the output space by 20%.
            // We should never run out of output space if
            // compress_format_version == 2
            size_t old_sz = output_len;
            uint32_t output_len_delta = output_len/5;
            output_len += output_len_delta < 10 ? 10 : output_len_delta;
            char* tmp = new char[output_len];
            memcpy(tmp, output, old_sz);
            delete[] output;
            output = tmp;

            // Set more output.
            _stream.next_out = (Bytef *)(output + old_sz);
            _stream.avail_out = static_cast<unsigned int>(output_len - old_sz);
            break;
        }
        case Z_BUF_ERROR:
        default:
            delete[] output;
            inflateEnd(&_stream);
            return nullptr;
        }
    }

    // If we encoded decompressed block size, we should have no bytes left
    assert(_stream.avail_out == 0);
    *decompress_size = static_cast<int>(output_len - _stream.avail_out);
    inflateEnd(&_stream);
    return output;
}

int Decompressor::Init() {
    assert(gzfileName.size() != 0);
    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    int ret = inflateInit2(&strm, WINDOW_BITS_GZIP);
    if (ret != Z_OK) {
        printf("failed to inflateInit2.");
        return ret;
    }

    source = fopen(gzfileName.c_str(), "r");
    if(source == nullptr) {
        printf("failed to open %s", gzfileName.c_str());
        return -1;
    }
    fseek(source, 0, SEEK_END);
    file_size = ftell(source);
    rewind(source);

    return 0;
}

int Decompressor::InitGZRead() {
    assert(gzfileName.size() != 0);

    /*get gz file size.*/
    source = fopen(gzfileName.c_str(), "r");
    if(source == nullptr) {
        printf("failed to open %s", gzfileName.c_str());
        return -1;
    }
    fseek(source, 0, SEEK_END);
    file_size = ftell(source);
    fclose(source);

    init_gzread = true;
    gzfp = gzopen(gzfileName.c_str(), "rb");
    if(gzfp == nullptr) {
        printf("failed to gzopen %s", gzfileName.c_str());
        return -2;
    }
    int ret = gzbuffer(gzfp, CHUNK);
    if (ret != 0) {
        printf("failed to init gzbuffer");
        return -3;
    }

    return 0;
}

int Decompressor::GZUncompressToFileByGZRead(std::string outfile) {
    printf("start to GZUncompressToFileByGZRead");
    if(outfile.size() == 0) {
        outfile = gzfileName.substr(0, gzfileName.find_last_of('.'));
    }
    FILE * dest = fopen(outfile.c_str(), "w");
    if (dest == NULL) {
        return -1;
    }

    int ret = 0;
    char *buf= new char[INFLATE_FACTOR * CHUNK];
    while(true) {
        ret = GZUncompressByGZRead(buf, INFLATE_FACTOR * CHUNK);
        if (ret < 0) {
            printf("failed to GZUncompressByGZRead, ret %d", ret);
            break;
        }

        int have = fwrite(buf, 1, ret, dest);
        if (have < 0) {
            printf("failed to fwirte , ret %d", ret);
            ret = -11;
            break;
        }
        fflush(dest);

        if (ret < INFLATE_FACTOR * CHUNK) {
            printf("finished to unzip  %s, ret %d", gzfileName.c_str(), ret);
            ret = 0;
            break;
        }
    }
    int fd = fileno(dest);
    if(fsync(fd) == -1) {
        printf("failed to fsync file %s.", outfile.c_str());
        ret = -12;
    }

    delete[] buf;
    fclose(dest);
    return ret;
}


int Decompressor::GZUncompressByGZRead(char * buf, size_t length) {
    int ret = 0;
    for(int retry = 0; retry < 3; retry++) {
        ret = gzread(gzfp, buf, length);
        if(ret < 0) {
            if (ret == Z_BUF_ERROR) {
                printf("gzread buf error, retry %d times, ret %d", retry, ret);
                gzclearerr(gzfp);
                if (retry == 2) {
                    printf("gzread fatal, buf error with retried 3 times. ret %d", ret);
                    break;
                }
                continue;
            }
            printf("gzread fatal. ret %d", ret);
            break;
        } else if (ret == Z_OK) {
            printf("gzread return with ok, retry again. ret %d.", ret);
            gzclearerr(gzfp);
            if (retry == 2) {
                printf("gzread returning Z_OK with retried 3 times. ret %d.",  ret);
                break;
            }
            continue;
        } else {
            decompressd_size += ret;
            size_t offset = gzoffset(gzfp);
            // printf("==> decompressed_size %ld, offset %ld ,gztell %ld.\n", decompressd_size, offset, gztell(gzfp));
            break;
        }
    }

    return ret;
}

int Decompressor::GZUncompressToFile(std::string outfile) {
    if(outfile.size() == 0) {
        outfile = gzfileName.substr(0, gzfileName.find_last_of('.'));
    }
    FILE * dest = fopen(outfile.c_str(), "w");
    if (dest == NULL) {
        return -1;
    }

    int ret = Z_OK;
    unsigned char *out = new unsigned char[INFLATE_FACTOR * CHUNK];
    /* decompress until deflate stream ends or end of file */
    do {
        int have = 0;
        ret  = GZUncompress(out, INFLATE_FACTOR * CHUNK, &have);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            printf("failed to GZUncompress, ret %d.", ret);
            ret = -2;
            break;
        }
        have = fwrite(out, 1, have, dest);
        if (have < 0) {
            printf("failed to fwirte , ret %d", ret);
            ret = -3;
            break;
        }

        if (ret == Z_STREAM_END) {
            printf("Z_STREAM_END");
        }
        fflush(dest);
        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);
    int fd = fileno(dest);
    if(fsync(fd) == -1) {
        printf("failed to fsync file %s.", outfile.c_str());
        ret = -4;
    }

    delete[] out;
    /* clean up and return */
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int Decompressor::GZUncompress(unsigned char * output_data, size_t output_length, int *decompress_size) {
    *decompress_size = 0;
    if(source == NULL) {
        return -1;
    }

    /* decompress until deflate stream ends or end of file */
    if(strm.avail_in == 0) {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0) {
            printf("no available gz data to read, may incomplete gz file %s.", gzfileName.c_str());
            (void)inflateEnd(&strm);
            return Z_STREAM_END;
        }
        if (strm.avail_in != CHUNK) {
            printf("last read count of read chunk %d each chunk size:%d strm.avail_in:%d.",
                chunk_read_counter, CHUNK, strm.avail_in);
        }
        chunk_read_counter++;
        strm.next_in = in;
    }

    /* run inflate() on input until output buffer not full */
    int ret = Z_OK;
    do {
        strm.avail_out = output_length;
        strm.next_out = output_data;
        ret = inflate(&strm, Z_SYNC_FLUSH);
        assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     /* and fall through */
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            printf("error with Z_NEED_DICT or Z_DATA_ERROR or Z_MEM_ERROR");
            return ret;
        }

        if (ret == Z_BUF_ERROR && strm.avail_in == 0) {
            // printf("CHUNK %d strm.avail_out %d strm.avail_in %d", CHUNK, strm.avail_out, strm.avail_in);
            ret = Z_OK;
            break;
        }

        decompressd_size += output_length - strm.avail_out;
        *decompress_size = output_length - strm.avail_out;
    } while (strm.avail_out != 0 && ret != Z_STREAM_END);

    if (ret == Z_STREAM_END) {
        (void)inflateEnd(&strm);
    }

    return ret ;
}