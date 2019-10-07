#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

struct ElasticPointerObj {
    int key;
    char val[1];
};

struct NormalPointerObj {
    int key;
    char *val;
};

int main()
{
    const char* name = "wangchenglin";
    ElasticPointerObj *e = (ElasticPointerObj*)malloc(sizeof(ElasticPointerObj) - 1 + strlen(name));
    NormalPointerObj *n = (NormalPointerObj*)malloc(sizeof(NormalPointerObj));
    n->val = (char *)malloc(strlen(name));
    memcpy(n->val, name, strlen(name));
    memcpy(e->val, name, strlen(name));

    printf("ElasticPointerObj val:%s\n", e->val);
    printf("NormalPointerObj val:%s\n", n->val);

    std::cout << "sizeof(*e) is " << sizeof(*e) << std::endl;
    std::cout << "sizeof(*n) is " << sizeof(*n) << std::endl;

    free(e);
    free(n->val);
    free(n);

    return 0;
}
