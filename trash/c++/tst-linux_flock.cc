#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

std::mutex io_mtx;

int fd = -1;
string file_path = "/Users/didi/code/cppsnippets/tst.meta";

int init() {
    fd =
        open(file_path.c_str(), O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR);
    std::cout << "init file_path=" << file_path << " opened. fd=" << fd
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    std::cout << "init file_path=" << file_path << " exit. fd=" << fd
              << std::endl;
    return 0;
}

int is_file_opened() {
    int check_fd =
        open(file_path.c_str(), O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR);
    int ret = fcntl(check_fd, F_GETFD);
    std::cout << "is_file_opened file_path=" << file_path << " ret=" << ret
              << " errno=" << errno << "\n";
    return 0;
}

int try_update_file(int id) {
    fd =
        open(file_path.c_str(), O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR);
    std::unique_lock<std::mutex> lock(io_mtx);
    std::cout << "\n id=" << id << " try lock file="
              << "tst.meta fd=" << fd << "\n";
    lock.unlock();
    flock(fd, LOCK_EX);
    lock.lock();
    std::cout << "\n id=" << id << " locked file="
              << "tst.meta\n";
    lock.unlock();
    write(fd, std::to_string(id).c_str(), 10);
    fsync(fd);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    close(fd);

    lock.lock();
    std::cout << errno << std::endl;
    lock.unlock();

    return 0;
}

// 加锁一个文件，并返回其fd，返回-1表示加锁失败
int TryLockFile(string file_path) {
    // 打开lock文件
    string lock_file = file_path + ".lock";
    int fd =
        open(lock_file.c_str(), O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("TryLockFile open lock_file=%s failed.\n", lock_file.c_str());
        return -1;
    }
    // 已非阻塞的方式加文件锁
    int ret = flock(fd, LOCK_EX | LOCK_NB);
    // int ret = flock(fd, LOCK_EX);
    if (ret == -1) {
        if (errno == EWOULDBLOCK) {
            // 已被加锁
            printf("TryLockFile lock_file=%s has been locked.\n",
                   lock_file.c_str());
            return 1;
        } else {
            // 加锁失败
            printf("TryLockFile lock_file=%s lock failed.\n",
                   lock_file.c_str());
            return -1;
        }
    } else {
        // 加锁成功
        printf("TryLockFile lock_file=%s errno=%d lock success.\n",
               lock_file.c_str(), errno);
        return 0;
    }
}

int UnLockFile(string file_path) {
    string lock_file = file_path + ".lock";
    int fd = open(lock_file.c_str(), O_WRONLY | O_SYNC);
    if (fd == -1) {
        printf("UnLockFile open lock_file=%s failed.\n", lock_file.c_str());
        return -1;
    }
    int ret = flock(fd, LOCK_UN | LOCK_NB);
    if (ret == -1) {
        printf("UnLockFile unlock lock_file=%s failed.\n", lock_file.c_str());
        return -2;
    }
    printf("UnLockFile lock_file=%s errno=%d unlock success.\n",
           lock_file.c_str(), errno);
    return 0;
}

int main() {
    TryLockFile("tst");
    UnLockFile("tst");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::thread th1(TryLockFile, "tst");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::thread th2(TryLockFile, "tst");

    th1.join();
    th2.join();

    return 0;
}
