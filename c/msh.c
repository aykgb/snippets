/*************************************************************************
    > File Name: mine.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Thu 16 Jun 2016 08:11:13 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LSH_RL_BUFSIZE 1024

char *lsh_read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

void lsh_loop()
{
    char *line;
    char **args;
    int status = 1;

    do {
        printf("LSH> ");
        line = lsh_read_line();
        if(strcmp(line, "\n"))
            printf(line);
        if(!strcmp(line, "exit\n"))
            break;
        free(line);
    } while(status);
}

int main(int argc, char **argv)
{
    lsh_loop();

    return 0;
}