// skiplist_test.cc
#include "skiplist.h"
#include <set>

#define ASSERT_EQ(M,N) assert((M) == (N))
#define ASSERT_TRUE(C) assert((C))

namespace jinger {

typedef uint64_t Key;

struct Comparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

} // namespace jinger

#include <time.h>
using namespace jinger;

int main(int argc, char** argv) {
  srandom(time(NULL));
  const int N = 65536;
  Comparator cmp;
  SkipList<Key, Comparator> list(cmp);
  std::set<int> s;
  for (int i = 0; i < N; i++) {
    Key key = random() % N;
    if(s.insert(key).second)
        list.Insert(key);
  }

  for (int i = 0; i < N; i++) {
    if (list.Contains(i)) {
      //ASSERT_EQ(keys.count(i), 1);
      printf("Contains %d\n", i);
    } else {
      //ASSERT_EQ(keys.count(i), 0);
      printf("Not contains %d\n", i);
    }
  }

}
