#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <glog/logging.h>

using namespace std;

std::atomic<int> state_;

std::vector<std::thread> thread_pool;

void work_func(int origin) {
  int target = origin + 1;
  while(!state_.compare_exchange_strong(origin, target));
  LOG(INFO) << "thread " << std::this_thread::get_id()
            << " update success, state_ is "
            << state_.load();
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;
  state_.store(0);

  int tmp = 0;
  auto ret = state_.compare_exchange_strong(tmp, 1);
  LOG(INFO) << "ret " << ret << " state " << state_.load();

  tmp = 2;
  ret = state_.compare_exchange_strong(tmp, 3);
  LOG(INFO) << "ret " << ret << " tmp "<< tmp <<  " state " << state_.load();

  while(!state_.compare_exchange_strong(tmp, 3));
  LOG(INFO) << "ret " << ret << " tmp "<< tmp <<  " state " << state_.load();

  state_.store(0);

  for(int i = 0; i < 128; i++) {
    thread_pool.emplace_back(std::thread(work_func, i));
  }

  for(int i = 0; i < thread_pool.size(); i++) {
      if(thread_pool[i].joinable()) {
          thread_pool[i].join();
      }
  }
  return 0;
}