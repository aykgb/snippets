/////////////////////////stringstream与snprintf性能对比测试///////////////////////////////
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
///////每次构建新的流对象
void* test_stringstream1(void * arg) {
    for(int i=0; i<10000; i++) {
        stringstream oss;////每次构建新的流对象代价很大,需要互斥地访问本地策略配置,多线程环境下互斥导致的代价更大
        oss << i;
    }
}
///////每次重用一个流对象,重用前清空
void* test_stringstream2(void * arg) {
    stringstream oss;
    for(int i=0; i<10000; i++) {
        oss.clear();////这仅仅置流标记
        oss.str("");/////这是才是真正清空操作
        oss << i;
    }
}
///////每次创建一个局部buf
void* test_snprintf1(void * arg) {
    for(int i=0; i<10000; i++) {
        char buf[1024];
        snprintf(buf, sizeof(buf), "%d", i);
    }
}
////////重用一块buf,重用前memset
void* test_snprintf2(void * arg) {
    char buf[1024];
    for(int i=0; i<10000; i++) {
        memset(buf,0x0,sizeof(buf));
        snprintf(buf, sizeof(buf), "%d", i);
    }
}

int main(int argc, char*argv[]) {
    timeval end;
    timeval start;
    uint32_t u32S;
    uint32_t u32E;
    for(int j=1; j<5; j++) {
        gettimeofday(&start,NULL);
        u32S=start.tv_sec*1000*1000 + start.tv_usec;
        vector<pthread_t> vtids1;
        for(int i=0; i<10; i++) {
            pthread_t ptid;
            pthread_create(&ptid,NULL,&test_stringstream1,NULL);
            vtids1.push_back(ptid);
        }
        for(vector<pthread_t> :: iterator iter=vtids1.begin(); iter!=vtids1.end(); iter++) {
            pthread_join(*iter,NULL);
        }
        gettimeofday(&end,NULL);
        u32E=end.tv_sec*1000*1000 + end.tv_usec;
        cout<<"test_stringstream1 :"<<u32E-u32S<< endl;

        gettimeofday(&start,NULL);
        u32S=start.tv_sec*1000*1000 + start.tv_usec;
        vector<pthread_t> vtids2;
        for(int i=0; i<10; i++) {
            pthread_t ptid;
            pthread_create(&ptid,NULL,&test_stringstream2,NULL);
            vtids2.push_back(ptid);
        }
        for(vector<pthread_t> :: iterator iter=vtids2.begin(); iter!=vtids2.end(); iter++) {
            pthread_join(*iter,NULL);
        }
        gettimeofday(&end,NULL);
        u32E=end.tv_sec*1000*1000 + end.tv_usec;
        cout<<"test_stringstream2 :"<<u32E-u32S<< endl;

        gettimeofday(&start,NULL);
        u32S=start.tv_sec*1000*1000 + start.tv_usec;
        vector<pthread_t> vtids3;
        for(int i=0; i<10; i++) {
            pthread_t ptid;
            pthread_create(&ptid,NULL,test_snprintf1,NULL);
            vtids3.push_back(ptid);
        }
        for(vector<pthread_t> :: iterator iter=vtids3.begin(); iter!=vtids3.end(); iter++) {
            pthread_join(*iter,NULL);
        }
        gettimeofday(&end,NULL);
        u32E=end.tv_sec*1000*1000 + end.tv_usec;
        cout<<"test_snprintf1     :"<<u32E-u32S<< endl;

        gettimeofday(&start,NULL);
        u32S=start.tv_sec*1000*1000 + start.tv_usec;
        vector<pthread_t> vtids4;
        for(int i=0; i<10; i++) {
            pthread_t ptid;
            pthread_create(&ptid,NULL,test_snprintf2,NULL);
            vtids4.push_back(ptid);
        }
        for(vector<pthread_t> :: iterator iter=vtids4.begin(); iter!=vtids4.end(); iter++) {
            pthread_join(*iter,NULL);
        }
        gettimeofday(&end,NULL);
        u32E=end.tv_sec*1000*1000 + end.tv_usec;
        cout<<"test_snprintf2     :"<<u32E-u32S<< endl;
    }
}
