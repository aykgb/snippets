#include <errno.h>
#include <sys/file.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>

#include <stdio.h>
#include <string.h>


using namespace std;

// 加锁一个文件，并返回其fd，返回-1表示加锁失败
class FileLocker {
    const size_t STATE_SIZE = 16;
  public:
    FileLocker(const string& fp) {
        if (fp.empty()) {
            lock_file = "default.lock";
        }
        lock_file = fp + ".lock";
    }

    ~FileLocker() {
        UnLock();
        reset();
    }
  private:
    int lfd = -1;
    string lock_file;
  public:
    int TryLock() {
        printf("this is %p\n", this);
        if (lfd != -1) {
            printf("TryLock lock_file=%s fd=%d had been locked by this object.\n", lock_file.c_str(), lfd);
        } else {
            // 打开lock文件
            lfd = open(lock_file.c_str(), O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR);
            if (lfd == -1) {
                printf("TryLockFile open lock_file=%s failed.\n", lock_file.c_str());
                return -1;
            }

        }
        // 以非阻塞的方式加文件锁
        int ret = flock(lfd, LOCK_EX | LOCK_NB);
        if (ret == -1) {
            if (errno == EWOULDBLOCK) {
                printf("TryLockFile lock_file=%s has been locked by another object.\n", lock_file.c_str());
                // 已被加锁
                reset();
                return 1;
            } else {
                // 加锁失败
                printf("TryLockFile lock_file=%s errno=%d lock failed.\n", lock_file.c_str(), errno);
                reset();
                return -1;
            }
        } else {
            printf("TryLockFile lock_file=%s lock success.\n", lock_file.c_str());
            // 加锁成功
            return 0;
        }
    }

    int UnLock() {
        if (lfd == -1) {
            printf("UnLock lock_file=%s not opened.\n", lock_file.c_str());
            return 0;
        }
        int ret = flock(lfd, LOCK_UN);
        if (ret == -1) {
            printf("UnLock lock_file=%s errno=%d unlock failed.\n", lock_file.c_str(), errno);
            return -1;
        }
        reset();
        printf("UnLock lock_file=%s unlock success.\n", lock_file.c_str());
        return 0;
    }

    int EncodeStatusToFile(int state) {
        if (lfd == -1) {
            // cannot be written without lock
            printf("EncodeStatusToFile lock_file=%s is unlock.\n", lock_file.c_str());
            return -1;
        }
        string st = std::to_string(state);
        write(lfd, st.c_str(), STATE_SIZE);
        fsync(lfd);

        return 0;
    }

    int DecodeStatusFromFile(int &state) {
        char st[STATE_SIZE];
        int fd = open(lock_file.c_str(), O_RDONLY | O_SYNC);
        if (fd == -1) {
            if (errno == ENOENT) {
                // file does not exist.
                state = 0;
                return 0;
            }
            printf("DecodeStatusFromFile open lock_file=%s failed.\n",
                   lock_file.c_str());
            return -1;
        }
        // 从lock文件中读出状态
        int ret = read(fd, st, STATE_SIZE);
        if (ret < 1) {
            state = 0;
            printf("DecodeStatusFromFile read lock_file=%s empty.\n",
                   lock_file.c_str());
            close(fd);
            return 0;
        }
        try {
            state = std::stol(string(st));
        } catch (std::exception& e) {
            printf("DecodeStatusFromFile lock_file=%s exception=%s.\n",
                   lock_file.c_str(), e.what());
            state = 0;
        }
        close(fd);

        return 0;
    }

  private:
    void reset() {
        if (lfd != -1) {
            close(lfd);
            lfd = -1;
        }
    }
};

void TEST_CASE_00() {
    int st;
    FileLocker flocker0("tst");
    FileLocker flocker1("tst");
    FileLocker flocker2("tst");
    std::cout << "==> flocker0 try lock.\n";
    flocker0.TryLock();
    std::cout << "==> flocker1 try lock.\n";
    flocker1.TryLock();

    flocker0.DecodeStatusFromFile(st);
    std::cout << "get st=" << st << "\n";
    flocker0.EncodeStatusToFile(1);

    sleep(1);
    std::cout << "==> flocker1 unlock.\n";
    flocker1.UnLock();
    std::cout << "==> flocker2 unlock.\n";
    flocker2.UnLock();
    std::cout << "==> flocker0 trylock.\n";
    flocker0.TryLock();
    std::cout << "==> flocker0 unlock.\n";
    flocker0.UnLock();
}

void TEST_CASE_01() {
    FileLocker flocker0("tst01");
    FileLocker flocker1("tst01");
    std::cout << "flocker0 trylock.\n";
    flocker0.TryLock();
    flocker0.TryLock();
    std::cout << "flocker0 unlock.\n";
    flocker0.UnLock();
    std::cout << "flocker1 trylock.\n";
    flocker1.TryLock();
}

void TEST_CASE_02() {
    FileLocker flocker0("tst01");
    FileLocker flocker1("tst01");
    std::cout << "flocker0 trylock.\n";
    flocker0.TryLock();
    flocker0.TryLock();
    std::cout << "flocker1 trylock.\n";
    flocker1.TryLock();
    std::cout << "flocker1 unlock.\n";
    flocker1.UnLock();
    flocker1.UnLock();
    std::cout << "flocker1 trylock.\n";
    flocker1.TryLock();
}

void TEST_CASE_03() {
    FileLocker flocker0("tst01");
    flocker0.TryLock();
    flocker0.TryLock();
    flocker0.TryLock();
    flocker0.TryLock();
    flocker0.TryLock();
}

void TEST_CASE_04() {
    FileLocker flocker0("tst01");
    FileLocker flocker1("tst01");
    std::cout << "flocker0 trylock.\n";
    flocker0.TryLock();
    flocker0.TryLock();
    std::cout << "flocker1 trylock.\n";
    flocker1.TryLock();
    std::cout << "flocker1 unlock.\n";
    flocker1.UnLock();
    flocker1.UnLock();
    std::cout << "flocker1 trylock.\n";
    flocker1.TryLock();
    std::cout << "flocker0 unlock.\n";
    flocker0.UnLock();
    std::cout << "flocker1 trylock.\n";
    flocker1.TryLock();
    flocker1.TryLock();
}

void TEST_CASE_05() {
    FileLocker flocker0("tst05");
    // FileLocker flocker1("tst05");
    std::thread th1(&FileLocker::TryLock, std::ref(flocker0));
    std::thread th2(&FileLocker::TryLock, std::ref(flocker0));
    std::thread th3(&FileLocker::TryLock, std::ref(flocker0));
    th1.join();
    th2.join();
    th3.join();
}

void TEST_CASE_06() {
    int fd = open("tst06.lock", O_WRONLY | O_CREAT | O_SYNC);
    write(fd, "A", 1);
    FileLocker flocker("tst06");
    int state;
    flocker.DecodeStatusFromFile(state);
    std::cout << "state=" << state << std::endl;
}

void TEST_CASE_07() {
    int fd = open("tst07.lock", O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR);
    write(fd, "A", 1);
    FileLocker flocker("tst07");
    int state = -1;
    std::cout << "state=" << state << std::endl;
    flocker.DecodeStatusFromFile(state);
    std::cout << "state=" << state << std::endl;
    flocker.EncodeStatusToFile(state + 1);
    std::cout << "state=" << state << std::endl;
    flocker.TryLock();
    std::cout << "state=" << state << std::endl;
    flocker.EncodeStatusToFile(state + 1);
    std::cout << "state=" << state << std::endl;
    flocker.DecodeStatusFromFile(state);
    std::cout << "state=" << state << std::endl;
}

int main() {
    std::cout << "==========> TEST_CASE_00 <========== \n";
    TEST_CASE_00();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "==========> TEST_CASE_01 <========== \n";
    TEST_CASE_01();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "==========> TEST_CASE_02 <========== \n";
    TEST_CASE_02();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "==========> TEST_CASE_03 <========== \n";
    TEST_CASE_03();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "==========> TEST_CASE_04 <========== \n";
    TEST_CASE_04();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "==========> TEST_CASE_05 <========== \n";
    TEST_CASE_05();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "==========> TEST_CASE_06 <========== \n";
    TEST_CASE_06();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "==========> TEST_CASE_07 <========== \n";
    TEST_CASE_07();
    return 0;
}
