#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define ERR_EXIT(m) \
  do\
  {\
    perror(m);\
    exit(EXIT_FAILURE);\
  }\
  while (0)\

void fun1()
{
    printf("fun1 is caled\n");
}

void fun2()
{
    printf("fun2 is called\n");
}

int main(void)
{
    pid_t pid;
    pid = fork();
    atexit(fun1);
    atexit(fun2);
    atexit(fun1);
    if(pid == -1)
        ERR_EXIT("fork error");
    if(pid == 0) {
        printf("This is child process\n");
    }
    if(pid > 0) {
        printf("This is patent process\n");
    }

    return 0;
}
