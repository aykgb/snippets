#include <math.h>

#include "common/common.h"
#include "common/time_util.h"

#include <thread>

namespace snippets {

class ChronoGraph {
 public:
  ChronoGraph(int64_t ttl_ms) : start_timepoint_(Now()), ttl_ms_(ttl_ms) {}

  bool HasTimeOut() { return RemainingTimeMS() == 0; }

  virtual int64_t Next() = 0;

 protected:
  int64_t RemainingTimeMS() {
    auto elapse_ms = ToMS(Now() - start_timepoint_);
    VLOG(10) << "RemainingTimeMS elapse_ms " << elapse_ms;
    if (elapse_ms > ttl_ms_) {
      return 0;
    }
    return ttl_ms_ - elapse_ms;
  }

 private:
  TimePoint start_timepoint_;
  int64_t ttl_ms_;
};

template <typename T = ChronoGraph,
          typename =
              typename std::enable_if_t<std::is_base_of<ChronoGraph, T>::value>>
class RetryPolicy {
 public:
  RetryPolicy(std::shared_ptr<T> graph) : chrono_graph_(graph) {}

  bool BackOffAndCanRetry() {
    SleepMS(chrono_graph_->Next());
    return !chrono_graph_->HasTimeOut();
  }

 private:
  std::shared_ptr<T> chrono_graph_;
};

class LinearChronoGraph : public ChronoGraph {
 public:
  LinearChronoGraph(int64_t ttl_ms, int64_t duration_num)
      : ChronoGraph(ttl_ms) {
    next_duration_ms_ = ttl_ms / duration_num;
    CHECK(next_duration_ms_ > 10);
    if (ttl_ms % duration_num) {
      next_duration_ms_ += 1;
    }
    VLOG(10) << "LinearChronoGraph next_duration_ms_ " << next_duration_ms_;
  }

  int64_t Next() override { return next_duration_ms_; }

 private:
  int64_t next_duration_ms_;
};

class ExponentialChronoGraph : public ChronoGraph {
 public:
  ExponentialChronoGraph(int64_t ttl_ms, int64_t duration_num,
                         double coefficient = 1.5)
      : ChronoGraph(ttl_ms), coefficient_(coefficient) {
    next_duration_ms_ =
        ttl_ms * (coefficient - 1) / (pow(coefficient, duration_num) - 1);
    CHECK(next_duration_ms_ > 10);
    if (ttl_ms % next_duration_ms_) {
      next_duration_ms_ += 1;
    }
  }

  int64_t Next() override {
    auto ret = next_duration_ms_;
    VLOG(10) << "ExponentialChronoGraph next_duration_ms_ "
             << next_duration_ms_;
    next_duration_ms_ = next_duration_ms_ * coefficient_;
    return ret;
  }

 private:
  int64_t next_duration_ms_;
  double coefficient_;
};

TEST(RetryPolicy, Test_Linear) {
  for (int i = 1; i < 10; i++) {
    auto chrono_graph = std::make_shared<LinearChronoGraph>(100, i);
    auto linear_retry_policy = RetryPolicy<>(chrono_graph);
    auto retry_times = 0;
    do {
      LOG(INFO) << "retry times: " << ++retry_times;
    } while (linear_retry_policy.BackOffAndCanRetry());
    ASSERT_EQ(retry_times, i);
  }
}

TEST(RetryPolicy, Test_Exponential) {
  for (int i = 1; i < 4; i++) {
    auto chrono_graph = std::make_shared<ExponentialChronoGraph>(100, i);
    auto exponential_retry_policy = RetryPolicy<>(chrono_graph);
    auto retry_times = 0;
    do {
      LOG(INFO) << "retry times: " << ++retry_times;
    } while (exponential_retry_policy.BackOffAndCanRetry());
    ASSERT_EQ(retry_times, i);
  }
}

}  // namespace snippets
