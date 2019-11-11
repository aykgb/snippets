// zlib
#include "include/zlib.h"

// std
#include <limits>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <chrono>

#include <assert.h>
#include <stdio.h>

using namespace std;

namespace gzip {

/* Decompress from file source to file dest until stream ends or EOF.
inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
allocated for processing, Z_DATA_ERROR if the deflate data is
invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
the version of the library linked do not match, or Z_ERRNO if there
is an error reading or writing the files. */

constexpr int CHUNK = 1024 * 512 * 1; // no more than 1M

int inf(FILE *source, FILE *dest) {
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

// The windowBits parameter is the base two logarithm of the window size (the size of the history buffer).
// It should be in the range 8..15 for this version of the library.
// Larger values of this parameter result in better compression at the expense of memory usage.
// This range of values also changes the decoding type:
//  -8 to -15 for raw deflate
//  8 to 15 for zlib
// (8 to 15) + 16 for gzip
// (8 to 15) + 32 to automatically detect gzip/zlib header
constexpr int window_bits = 15 + 32; // auto with windowbits of 15

int infgz(FILE *source, FILE *dest) {
    fseek(source, 0L, SEEK_END);
    int filesize = ftell(source);
    fseek(source, 0L, SEEK_SET);

    std::cout << "filesize: " << filesize << std::endl;

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit2(&strm, window_bits);
    if (ret != Z_OK) {
        return ret;
    }

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int infgz_no_output(FILE *source) {
    fseek(source, 0L, SEEK_END);
    int filesize = ftell(source);
    fseek(source, 0L, SEEK_SET);

    std::cout << "filesize: " << filesize << std::endl;

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];


    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit2(&strm, window_bits);
    if (ret != Z_OK) {
        return ret;
    }
    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);


    std::cout << "have " << have << std::endl;
    std::cout << "======>\n";
    char bbuf[CHUNK];
    snprintf(bbuf, have, "%s", out);

    std::cout << "using std::getline\n";
    std::istringstream istream(string(bbuf, CHUNK));
    std::string data;
    while(std::getline(istream, data)) {
        std::cout << data << "\n";
    }

    std::cout << "using printf\n";
    printf("%s\n", bbuf);

    /* clean up and return */
    (void)inflateEnd(&strm);

    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

constexpr int GZBUFF_SIZE = 1024 * 1024;
constexpr int GZREAD_SIZE = 1024 * 256;

int infgz_use_gzf(const char *filename) {
    gzFile fp = gzopen(filename, "rb");

    int ret = gzbuffer(fp, GZBUFF_SIZE);
    if (ret == -1) {
        std::cout << "gzbuffer method failed." << std::endl;
        return ret;
    }

    int retry = 0;
    int last_size = 0;
    unsigned char buf[GZREAD_SIZE];
    while(true) {
        ret = gzread(fp, buf, GZREAD_SIZE);
        if(ret < 0) {
            if (ret == Z_BUF_ERROR) {
                std::cout <<  "gzread buf error, retry again. ret " << ret << std::endl;
                gzclearerr(fp);
                retry++;
                if (retry == 3) {
                    std::cout <<  "gzread fatal, buf error with retried 3 times. ret " << ret << std::endl;
                    return ret;
                }
                continue;
            }
            std::cout <<  "gzread fatal. ret " << ret << std::endl;
            return ret;
        } else if (ret == Z_OK) {
            std::cout <<  "gzread return with ok, retry again. ret " << ret << std::endl;
            gzclearerr(fp);
            retry++;
            if (retry == 3) {
                break;
            }
            continue;
        } else if (ret < GZREAD_SIZE) {
            last_size = ret;
            break;
        } else {
            // normal
            last_size = ret;
        }
    }

    std::cout << "last_size " << last_size << std::endl;
    std::cout << "======>\n";
    char bbuf[1024 * 64];
    snprintf(bbuf, last_size, "%s", buf);
    printf("%s\n", bbuf);

    gzclose(fp);
    return ret;
}

#include <string>
#include <iostream>

extern "C" {
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "include/zlib.h"
}

using namespace std;

int test00() {
    char text[] = "zlib compress and uncompress test\nturingo@163.com\n2012-11-05\n";
    uLong tlen = strlen(text) + 1;		/* 需要把字符串的结束符'\0'也一并处理 */
    char* buf = NULL;
    uLong blen;

    /* 计算缓冲区大小，并为其分配内存 */
    blen = compressBound(tlen);			/* 压缩后的长度是不会超过blen的 */
    if((buf = (char*)malloc(sizeof(char) * blen)) == NULL) {
        printf("no enough memory!\n");
        return -1;
    }

    /* 压缩 */
    if(compress(reinterpret_cast<Bytef*>(buf), &blen, reinterpret_cast<Bytef*>(text), tlen) != Z_OK) {
        printf("compress failed!\n");
        return -1;
    }

    /* 解压缩 */
    if(uncompress(reinterpret_cast<Bytef*>(text), &tlen, reinterpret_cast<Bytef*>(buf), blen) != Z_OK) {
        printf("uncompress failed!\n");
        return -1;
    }

    /* 打印结果，并释放内存 */
    printf("%s", text);
    if(buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return 0;
}


bool gzipInflate( const std::string& compressedBytes, std::string& uncompressedBytes ) {
    if ( compressedBytes.size() == 0 ) {
        uncompressedBytes = compressedBytes ;
        return true ;
    }

    uncompressedBytes.clear() ;

    unsigned full_length = compressedBytes.size() ;
    unsigned half_length = compressedBytes.size() / 2;

    unsigned uncompLength = full_length ;
    char* uncomp = (char*) calloc( sizeof(char), uncompLength );

    z_stream strm;
    strm.next_in = (Bytef *) compressedBytes.c_str();
    strm.avail_in = compressedBytes.size() ;
    strm.total_out = 0;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;

    bool done = false ;

    if (inflateInit2(&strm, (16+MAX_WBITS)) != Z_OK) {
        free( uncomp );
        return false;
    }

    while (!done) {
        // If our output buffer is too small
        if (strm.total_out >= uncompLength ) {
            // Increase size of output buffer
            char* uncomp2 = (char*) calloc( sizeof(char), uncompLength + half_length );
            memcpy( uncomp2, uncomp, uncompLength );
            uncompLength += half_length ;
            free( uncomp );
            uncomp = uncomp2 ;
        }

        strm.next_out = (Bytef *) (uncomp + strm.total_out);
        strm.avail_out = uncompLength - strm.total_out;

        // Inflate another chunk.
        int err = inflate (&strm, Z_SYNC_FLUSH);
        if (err == Z_STREAM_END) done = true;
        else if (err != Z_OK)  {
            break;
        }

    }

    if (inflateEnd (&strm) != Z_OK) {
        free( uncomp );
        return false;
    }

    std::cout << "strm.total_out " << strm.total_out << "\n";
    for ( size_t i=0; i<strm.total_out; ++i ) {
        uncompressedBytes += uncomp[ i ];
    }
    free( uncomp );
    return true ;
}

/* Reads a file into memory. */
bool loadBinaryFile( const std::string& filename, std::string& contents ) {
    // Open the gzip file in binary mode
    FILE* f = fopen( filename.c_str(), "rb" );
    if ( f == NULL )
        return false ;

    // Clear existing bytes in output vector
    contents.clear();

    // Read all the bytes in the file
    int c = fgetc( f );
    while ( c != EOF) {
        contents +=  (char) c ;
        c = fgetc( f );
    }
    fclose (f);

    return true ;
}

int test01() {
    // Read the gzip file data into memory
    std::string fileData ;
    if ( !loadBinaryFile( "gizzle_queue_neighbor.gz", fileData ) ) {
        printf( "Error loading input file." );
        return 0 ;
    }

    // Uncompress the file data
    std::string data ;
    if ( !gzipInflate( fileData, data ) ) {
        printf( "Error decompressing file." );
        return 0 ;
    }

    // Print the data
    printf( "Data: \"" );
    for ( size_t i=0; i<data.size(); ++i ) {
        printf( "%c", data[i] );
    }
    printf ( "\"\n" );
    return 0;
}

class Decompressor {
    std::size_t max_;

  public:
    Decompressor(std::size_t max_bytes = 1000000000) // by default refuse operation if compressed data is > 1GB
        : max_(max_bytes) {
    }

    template <typename OutputType>
    void decompress(OutputType& output,
                    const char* data,
                    std::size_t size) const {
        z_stream inflate_s;

        inflate_s.zalloc = Z_NULL;
        inflate_s.zfree = Z_NULL;
        inflate_s.opaque = Z_NULL;
        inflate_s.avail_in = 0;
        inflate_s.next_in = Z_NULL;

        // The windowBits parameter is the base two logarithm of the window size (the size of the history buffer).
        // It should be in the range 8..15 for this version of the library.
        // Larger values of this parameter result in better compression at the expense of memory usage.
        // This range of values also changes the decoding type:
        //  -8 to -15 for raw deflate
        //  8 to 15 for zlib
        // (8 to 15) + 16 for gzip
        // (8 to 15) + 32 to automatically detect gzip/zlib header
        constexpr int window_bits = 15 + 32; // auto with windowbits of 15

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
        if (inflateInit2(&inflate_s, window_bits) != Z_OK) {
            throw std::runtime_error("inflate init failed");
        }
#pragma GCC diagnostic pop
        inflate_s.next_in = (Bytef*)data;

#ifdef DEBUG
        // Verify if size (long type) input will fit into unsigned int, type used for zlib's avail_in
        std::uint64_t size_64 = size * 2;
        if (size_64 > std::numeric_limits<unsigned int>::max()) {
            inflateEnd(&inflate_s);
            throw std::runtime_error("size arg is too large to fit into unsigned int type x2");
        }
#endif
        if (size > max_ || (size * 2) > max_) {
            inflateEnd(&inflate_s);
            throw std::runtime_error("size may use more memory than intended when decompressing");
        }
        inflate_s.avail_in = static_cast<unsigned int>(size);
        std::size_t size_uncompressed = 0;
        do {
            std::size_t resize_to = size_uncompressed + 2 * size;
            if (resize_to > max_) {
                inflateEnd(&inflate_s);
                throw std::runtime_error("size of output string will use more memory then intended when decompressing");
            }
            output.resize(resize_to);
            inflate_s.avail_out = static_cast<unsigned int>(2 * size);
            inflate_s.next_out = reinterpret_cast<Bytef*>(&output[0] + size_uncompressed);
            int ret = inflate(&inflate_s, Z_FINISH);
            if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
                std::string error_msg = inflate_s.msg;
                inflateEnd(&inflate_s);
                throw std::runtime_error(error_msg);
            }

            size_uncompressed += (2 * size - inflate_s.avail_out);
        } while (inflate_s.avail_out == 0);
        inflateEnd(&inflate_s);
        output.resize(size_uncompressed);
    }
};

inline std::string decompress(const char* data, std::size_t size) {
    Decompressor decomp;
    std::string output;
    decomp.decompress(output, data, size);
    return output;
}

} // namespace gzip
