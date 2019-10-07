#pragma once

#include <unistd.h>
#include <string>

using namespace std;

namespace util {

  const int32_t LOCK_SUCCESS = 0;
class FileLocker {
  const size_t  MAX_STATE_SIZE = 16;
  const int32_t INIT_STATUS = 0;

public:
  FileLocker(const string &fp) {
    if (fp.empty()) {
      lock_file = "default.lock";
    }
    lock_file = fp + ".lock";
  }

  ~FileLocker() {
    UnLock();
    reset();
  }

public:
  int TryLock();
  int UnLock();
  int EncodeStatusToFile(int status);
  int DecodeStatusFromFile(int &status);

private:
  string lock_file;
  int lfd = -1;

private:
   inline void reset() {
       if (lfd != -1) {
           close(lfd);
           lfd = -1;
       }

       return;
   }
};

} // namespace util
