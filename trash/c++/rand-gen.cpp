#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void rand_gen(vector<int>& tmp, int n)
{
    srand(time(NULL));
    for(int i = 0; i < n; ++i) {
        tmp.push_back(rand());
    }

    // for (int i = 0; i < num; ++i)
    // {
    // temp.push_back(i + 1);
    // }
    // random_shuffle(temp.begin(), temp.end());
}

int main(int argc, char *argv[])
{
    if(argc < 2) exit(-1);
    int num = atoi(argv[1]);
    vector<int> temp;

    rand_gen(temp, num);

    // for (size_t i = 0; i < temp.size(); i++)
    // {
    // printf("%d  ", temp[i]);
    // }

    FILE *hd_file = fopen("rand.txt", "w+");
    if(hd_file == NULL) {
        printf("Open a file failed!\n");
        exit(-1);
    }
    for(size_t i = 0; i < temp.size(); ++i) {
        fprintf(hd_file, "%d ", temp[i]);
    }

    // rewind(hd_file);
    // for(size_t i = 0; i < temp.size(); ++i)
    // {
    // int tmp;
    // fscanf(hd_file, "%d ", &tmp);
    // printf("%d ", tmp);
    // }

    return 0;
}
