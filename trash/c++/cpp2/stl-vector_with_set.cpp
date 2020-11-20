#include <string>
#include <vector>
#include <set>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>

using namespace std;

void print_trace (int signo)
{
    void *bktrace[50];
    size_t size;
    char **strings;
    size_t i;
    size = backtrace (bktrace, 50);
    /*backtrace_symbols_fd(bktrace, 10, memAllocFd);*/
    strings = backtrace_symbols (bktrace, size);

    printf("**********Start to print back trace info ********\r\n");
    for (i = 0; i < size; i++)
        printf ( "%s\n", strings[i]);

    for (i = 0; i < size; i++)
        printf ( "%s\n", strings[i]);

    free (strings);
}


// using namespace std;
int main()
{
//  signal(SIGSEGV, print_trace);
    std::vector<set<int> *> vec_pSet;
//  set<int> *s0 = new std::set<int>;
//  set<int> *s1 = new std::set<int>;
//  set<int> *s2 = new std::set<int>;
//  set<int> *s3 = new std::set<int>;
//  printf("s0 is %p\n", s0);
//  printf("s1 is %p\n", s1);
//  vec_pSet.push_back(s0);
//  vec_pSet.push_back(s1);
//  vec_pSet.push_back(s2);
//  vec_pSet.push_back(s3);'

    vec_pSet.push_back(new std::set<int>);
//  vec_pSet.push_back(new std::set<int>);

// for(int i = 0; i < 10000; i++)
// {
    std::set<int>::iterator its, its1;
    vec_pSet[0]->insert(0);
    vec_pSet[0]->insert(1);
    vec_pSet[0]->insert(3);
    vec_pSet[0]->insert(10);
    vec_pSet[0]->insert(12);

    its = vec_pSet[0]->find(0);
    its1 = its;
    cout << *its << endl;
    //}

//  printf("%p\n", vec_pSet[0]);
//  printf("%p\n", vec_pSet[1]);

//  printf("The set size is: %d\n", vec_pSet[0]->size());
//  std::cout << vec_pSet[0]->max_size() << std::endl;

//  vec_pSet[0]->insert(1);
//  vec_pSet[0]->insert(2);
    delete vec_pSet[0];
//  printf("After \"delete vec_pSet[0]\", the vec_pSet[0] is %p\n", vec_pSet[0]);

//  vec_pSet[0]->erase(1);
//  if(!vec_pSet[0]->empty())
//  {
//    printf("\"vec_pSet[0]->empty()\" is running OK!\n");
//  }
//  printf("The set size is: %d\n", vec_pSet[0]->size());

//  printf("Now! let us run the code \"vec_pSet[0]->find(1);\"...\n");

    if(vec_pSet[0]->find(0) != vec_pSet[0]->end()) {
        printf("find(0) is OK!\n");
    }

//  printf("insert(1)\n");
//  vec_pSet[0]->insert(1);

    printf("insert(0)\n");
    vec_pSet[0]->insert(0);
    vec_pSet[0]->insert(1);

    vec_pSet[0]->erase(0);
    vec_pSet[0]->erase(2);

//  printf("erase(10001)\n");
//  vec_pSet[0]->erase(10001);

    return 0;
}
