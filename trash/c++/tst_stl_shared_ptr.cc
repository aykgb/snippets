#include <stdio.h>
#include <iostream>
#include <memory>

using namespace std;

int main() {
  std::shared_ptr<int> p0(new int(1));
  std::shared_ptr<int> p1 = p0;
  printf("p0:%d, p1:%d\n", *p0, *p1);

  p1.reset(new int(2));
  printf("p0:%d, p1:%d\n", *p0, *p1);

  std::shared_ptr<int>& p2 = p1;
  p2.reset(new int(3));

  printf("p0:%d, p1:%d, p2:%d\n", *p0, *p1, *p2);

  return -1;
}
