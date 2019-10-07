#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>     // std::cout
#include <algorithm>    // std::transform

typedef int (*sdkFuncPtr)(unsigned int id, ...);
typedef int (*sdkFuncPtr2)();

typedef struct {
    const char *str;
    sdkFuncPtr ptr;
} funStru;

//sdkFuncPtr f1;
//sdkFuncPtr f2;

int adpt_oduksw_latency_get(unsigned int id, int pa, int pb, int pc)
{
    std::cout << id << std::endl;
    std::cout << pa << std::endl;
    std::cout << pb << std::endl;
    std::cout << pc << std::endl;

    return 0;
}

funStru funArray[] = {
    {"adpt_oduksw_latency_get", (sdkFuncPtr)adpt_oduksw_latency_get},
};

int main()
{
    printf("%s, %p\n", funArray[0].str, funArray[0].ptr);

    return 0;
}
