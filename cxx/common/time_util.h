#pragma once

#include <chrono>
#include <thread>

namespace snippets {

using TimePoint = std::chrono::steady_clock::time_point;

inline TimePoint Now() { return std::chrono::steady_clock::now(); }

template <typename T, typename = typename std::enable_if_t<
                          std::chrono::__is_duration<T>::value>>
inline int64_t ToMS(T duration) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
      .count();
}

inline void SleepUS(int64_t duration_us) {
  std::this_thread::sleep_for(std::chrono::microseconds(duration_us));
}

inline void SleepMS(int64_t duration_ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
}

class Timestamp {
 public:
  static inline int64_t ns() {
    return std::chrono::system_clock::now().time_since_epoch().count();
  }

  static inline int64_t us() { ns() / 1000; }

  static inline int64_t ms() { us() / 1000; }

  static inline int64_t sec() { ms() / 1000; }
};

}  // namespace snippets
