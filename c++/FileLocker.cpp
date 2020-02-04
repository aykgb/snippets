#include <errno.h>
#include <sys/file.h>
#include <unistd.h>

#include <string.h>
#include <string>
#include <glog/logging.h>

#include "FileLocker.h"

using namespace std;

namespace util {
// 加锁一个文件，并返回其fd，返回-1表示加锁失败
int FileLocker::TryLock() {
  if (lfd != -1) {
    // INFO("TryLock lock_file=%s lfd=%d had been locked by this object.",
            //  lock_file.c_str(), lfd);
  } else {
    // 打开lock文件
    lfd =
        open(lock_file.c_str(), O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR);
    if (lfd == -1) {
      // log_error("TryLock lock_file=%s errno=%d failed.", lock_file.c_str(),
                // errno);
      return -1;
    }
  }
  // 以非阻塞的方式加文件锁
  int ret = flock(lfd, LOCK_EX | LOCK_NB);
  if (ret == -1) {
    if (errno == EWOULDBLOCK) {
      // log_info("TryLock lock_file=%s has been locked by another object.",
              //  lock_file.c_str());
      // 已被加锁
      reset();
      return 1;
    } else {
      // 加锁失败
      reset();
      // log_info("TryLock lock_file=%s errno=%d lock failed.", lock_file.c_str(),
              //  errno);
      return -1;
    }
  } else {
    // log_info("TryLock lock_file=%s lfd=%d lock succes.", lock_file.c_str(),
            //  lfd);
    // 加锁成功 注：同一个fd重复加锁，会加锁成功。
    return 0;
  }
}

int FileLocker::UnLock() {
  if (lfd == -1) {
    // log_info("UnLock lock_file=%s not opened.", lock_file.c_str());
    return 0;
  }
  int ret = flock(lfd, LOCK_UN);
  if (ret == -1) {
    // log_error("UnLock lock_file=%s errno=%d unlock failed.", lock_file.c_str(),
              // errno);
    return -1;
  }
  // log_info("UnLock lock_file=%s unlock success.", lock_file.c_str());
  reset();
  return 0;
}

int FileLocker::EncodeStatusToFile(int state) {
  if (lfd == -1) {
    // cannot be written without lock
    // log_error("EncodeStatusToFile lock_file=%s is unlock.", lock_file.c_str());
    return -1;
  }
  string st = std::to_string(state);
  lseek(lfd, 0, SEEK_SET);
  write(lfd, st.c_str(), st.size());
  fsync(lfd);

  return 0;
}

int FileLocker::DecodeStatusFromFile(int &status) {
  char st[MAX_STATE_SIZE];
  memset(st, 0, MAX_STATE_SIZE * sizeof(char));
  int fd = open(lock_file.c_str(), O_RDONLY | O_SYNC);
  if (fd == -1) { // 文件不存在的情况
    if (errno == ENOENT) {
      // log_info("DecodeStatusFromFile lock_file=%s does not exist.\n",
              //  lock_file.c_str());
      // file does not exist.
      status = INIT_STATUS;
      return 0;
    }
    // log_error("DecodeStatusFromFile open lock_file=%s errno=%d failed.",
              // lock_file.c_str(), errno);
    return -1;
  }

  // 从lock文件中读出状态
  int ret = read(fd, st, MAX_STATE_SIZE);
  if (ret < 1) {
    // log_info("DecodeStatusFromFile read lock_file=%s empty ret=%d errno=%d.\n",
            //  lock_file.c_str(), ret, errno);
    status = INIT_STATUS;
    close(fd);
    return 0;
  }

  // 读出状态并防止异常退出
  try {
    status = std::stol(string(st));
  } catch (std::exception& e) {
    // log_info("DecodeStatusFromFile lock_file=%s exception=%s.\n",
            //  lock_file.c_str(), e.what());
    status = INIT_STATUS;
  }
  close(fd);

  return 0;
}

} // namespace util

