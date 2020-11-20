#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>

int main() {
  std::string uri =
      "/Users/wangchenlin/Code/github/aykgb/snippets/opened_file";
  int fd = open(uri.c_str(), O_CREAT | O_RDWR, 777);

  std::string buf = "0000000|1111111|2222222|";
  auto writed = write(fd, buf.c_str(), 24);
  auto ret = fsync(fd);
  printf("sycn resutl %d\n", ret);
  close(fd);

  char content[1024];
  memset(content, 0, 1024);

  fd = open(uri.c_str(), O_RDONLY);
  auto readed = read(fd, content, 24);
  printf("readed %d size, content =====>%s<=====\n", readed, content);
  ret = close(fd);
  printf("close resutl %d\n", ret);


  return 0;
}