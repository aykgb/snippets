#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

void swap(int& a, int& b)
{
    a = a - b;
    b = b + a;
    a = b - a;
}

void bubble(vector<int>& vec)
{
    size_t k = vec.size();
    /* Variable i is used to eliminate the scope of traversal. */
    for(size_t i = k; i > 0 && k >0; --i) {
        /* Variable k is used to record the position of the last element
         * which is located on the traverse range boundaries.
         * And it's also regarded as a flag of which indicate the sorting may be finished.
         */
        k = 0;
        for(size_t j = 1; j < i; ++j) {
            if(vec[j] < vec[j - 1]) {
                swap(vec[j], vec[j - 1]);
                k = j;
            }
        }
    }
}

void insertion(vector<int>& vec)
{
    for(size_t i = 1; i < vec.size(); ++i) {
        // size_t loca_insert = 0;
        // /* Find the location */
        // for(; loca_insert < i && vec[i] > vec[loca_insert]; ++loca_insert);
        // /* Shift the array and insert the element*/
        // int ele = vec[i];
        // for(size_t j = i; j > loca_insert; vec[j] = vec[j - 1] ,--j);
        // vec[loca_insert] = ele;

        for(size_t j = i; j > 0 && vec[j] < vec[j - 1] ; --j)
            swap(vec[j], vec[j - 1]);
    }
}

void shell(vector<int>& vec)
{
    for(size_t gap = vec.size()/2; i > 0; gap /= 2)
        for(size_t i = 0; i < )
        }

int main()
{
    FILE *hd_file = fopen("rand.txt", "r");
    if(hd_file == NULL) {
        printf("Open rand.txt file failed!\n");
        exit(-1);
    }

    int tmp;
    vector<int> vec;
    while(fscanf(hd_file, "%d ", &tmp) != EOF) {
        vec.push_back(tmp);
    }

    printf("----- This is the random sequence. -----\n");
    for(size_t i = 0; i < vec.size(); ++i) {
        printf("%-10d", vec[i]);
    }

    insertion(vec);
    // bubble(vec);

    printf("----- This is the order sequence. -----\n");
    for(size_t i = 0; i < vec.size(); ++i) {
        printf("%-10d", vec[i]);
    }

    return 0;
}