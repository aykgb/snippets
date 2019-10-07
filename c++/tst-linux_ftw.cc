#include <errno.h>
#include <ftw.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

long long int totalDirectorySize;

int sumDirectory(const char *fpath, const struct stat *sb, int typeflag) {
    (void)fpath;
    (void)typeflag;
    totalDirectorySize += sb->st_size;
    return 0;
}

long long int GetDirectorySize(const char *dir) {
    totalDirectorySize = 0;

    if (!dir || access(dir, R_OK)) {
        return -1;
    }

    if (ftw(dir, &sumDirectory, 1)) {
        perror("ftw");
        return -2;
    }

    return totalDirectorySize;
}

int main(int argc, char **argv) {
    long long int total = GetDirectorySize(argv[1]);

    printf("%s: %lld\n", argv[1], total);
    return 0;
}
