#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

int main() {
  std::string lstr = "abcd";
  std::string rstr = "";

  std::cout << memcmp(lstr.data(), rstr.data(), 0) << std::endl;

  return 0;
}
