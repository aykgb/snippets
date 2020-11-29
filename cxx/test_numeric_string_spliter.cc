#include <string>
#include <vector>

#include "common/common.h"

namespace snippets {

class NumericStringSpliter {
 public:
  explicit NumericStringSpliter(int base = 10) : base_(base) {
    CHECK(base_ == 10 || base_ == 16);
    auto last_row = base_ == 10 ? "99999999" : "FFFFFFFF";
    last_row_int_ = std::stoll(last_row, nullptr, base_);
  }

  std::vector<std::string> Split(int n) {
    if (n < 1 || n > last_row_int_ >> 3) {
      LOG(ERROR) << "Split parameter error";
      CHECK(false);
    }
    std::vector<std::string> result;
    int64_t step = last_row_int_ / n;
    int64_t current = step;
    for (int64_t i = 0; i < n - 1; ++i, current += step) {
      result.emplace_back(NumbericToString(current, NUMERIC_SIZE));
    }
    return result;
  }

 private:
  static constexpr int32_t NUMERIC_SIZE = 8;

  std::string NumbericToString(int64_t number, int total_len) {
    std::string number_str;
    number_str.resize(total_len);
    std::string format =
        "%0" + std::to_string(total_len) + (base_ == 10 ? "d" : "x");
    auto used = snprintf(&number_str[0], total_len, format.c_str(), number);
    CHECK(used == total_len);
    VLOG(10) << "number_str:" << number_str;
    return number_str;
  }

  int base_;
  int64_t last_row_int_{0};
};

TEST(NumericStringSpliter, Test_00) {
  NumericStringSpliter spliter;
  auto res = spliter.Split(100);
  ASSERT_EQ(res.size(), 99);
}

}  // namespace snippets
