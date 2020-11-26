#include "common/common.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  return RUN_ALL_TESTS();
}
