#include <type_traits>

#include "common/common.h"

namespace snippets {

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type is_odd(T i) {
  return static_cast<bool>(i % 2);
}
// 如果std::is_integral<T>::value 是 true，则std::enable_if<std::is_integral<T>,
// bool>::type 是 bool类型，否则编译不通过

template <typename T,
          typename = typename std::enable_if_t<std::is_integral<T>::value>>
bool is_even(T i) {
  return !static_cast<bool>(i % 2);
}
// 如果std::is_integral<T>::value
// 是true，则std::enable_if<std::is_integral<T>::value>::type
// 是void(默认模板参数)，否则报错

TEST(EnableIf, Test_00) {
  // is_odd(1.0);
  is_odd(1);
  // is_even(1.0);
  is_even(2);
}

}  // namespace snippets
