#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void do_print_id(int id) {
  std::unique_lock<std::mutex> lck(mtx);
  std::cout << "function id: " << id << "is waiting for lck ..." << std::endl;

  while (!ready) cv.wait(lck);

  std::cout << "function id: " << id << " is finished." << std::endl;
}

void go() {
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}

int main() {
  auto thread_nums = 5;
  std::vector<std::thread> threads(thread_nums);

  for (int i = 0; i < thread_nums; ++i) {
    threads[i] = std::thread(do_print_id, i);
  }

  std::cout << "thread_nums " << thread_nums << "threads ready to race ..."
            << std::endl;

  go();

  for (int i = 0; i < thread_nums; ++i) {
      if (threads[i].joinable()) { // !must verify joinable condition.
          threads[i].join();
      }
  }

  return 0;
}
