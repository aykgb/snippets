// static_pointer_cast example
#include <iostream>
#include <memory>

std::shared_ptr<const int> func() {
    auto val = std::make_shared<int>(30);
    return val;
}

int main () {
  std::shared_ptr<const int> fo(new int(10));
  std::shared_ptr<const int> foo;
  foo = fo;
  std::cout << "*foo: " << *foo << '\n';
  auto bar = std::const_pointer_cast<int>(foo);
  *bar = 20;
  std::cout << "*foo: " << *foo << '\n';

  auto fooo = func();

  std::cout << "*fooo: " << *fooo << std::endl;
  /*
  try {
    *fooo = 40;
  } catch(std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  */

  return 0;
}
