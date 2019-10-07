#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    int fds[2];
    if(pipe(fds) == -1) {
        perror("Pipe error.");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if(pid == -1) {
        perror("fork error.");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) {
        close(fds[0]);
        write(fds[1], "hello", 5);
        exit(EXIT_FAILURE);
    }

    close(fds[1]);
    char buf[10] = {0};
    read(fds[0], buf, 10);
    printf("Receive data = %s\n", buf);
    return 0;
}
