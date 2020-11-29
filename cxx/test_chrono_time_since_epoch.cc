#include <chrono>
#include <iostream>
#include <string>

#include "common/common.h"

namespace snippets {

TEST(chrono, TimeSinceEpoch) {
  auto now = std::chrono::steady_clock::now().time_since_epoch();
  auto elapse_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now);
  auto elapse_s = std::chrono::duration_cast<std::chrono::seconds>(now);
  LOG(INFO) << "steady_clock time since epoch " << elapse_ms.count() << " ms"
            << std::endl;
  LOG(INFO) << "steady_clock time since epoch " << elapse_s.count() << " s"
            << std::endl;

  now = std::chrono::system_clock::now().time_since_epoch();
  elapse_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now);
  elapse_s = std::chrono::duration_cast<std::chrono::seconds>(now);
  LOG(INFO) << "system_clock time since epoch " << elapse_ms.count() << " ms"
            << std::endl;
  LOG(INFO) << "system_clock time since epoch " << elapse_s.count() << " s"
            << std::endl;
}

}  // namespace snippets
