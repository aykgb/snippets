#include <iostream>
#include <bitset>
#include <cstdlib> // rand
#include <ctime> // timer

inline unsigned long get_time_in_ms()
{
    return (unsigned long)((double(clock()) / CLOCKS_PER_SEC) * 1000);
}


void one_sec_delay()
{
    unsigned long end_time = get_time_in_ms() + 1000;

    while(get_time_in_ms() < end_time) {
    }
}



int main(int argc, char **argv)
{
    srand(get_time_in_ms());

    using namespace std;

    bitset<5000000> bits;
    bool *bools = new bool[5000000];

    unsigned long current_time, difference1, difference2;
    double total;

    one_sec_delay();

    total = 0;
    current_time = get_time_in_ms();

    for (unsigned int num = 0; num != 200000000; ++num) {
        bools[rand() % 5000000] = rand() % 2;
    }

    difference1 = get_time_in_ms() - current_time;
    current_time = get_time_in_ms();

    for (unsigned int num2 = 0; num2 != 100; ++num2) {
        for (unsigned int num = 0; num != 5000000; ++num) {
            total += bools[num];
        }
    }

    difference2 = get_time_in_ms() - current_time;

    cout << "Bool:" << endl << "sum total = " << total << ", random access time = " << difference1 << ", sequential access time = " << difference2 << endl << endl;


    one_sec_delay();

    total = 0;
    current_time = get_time_in_ms();

    for (unsigned int num = 0; num != 200000000; ++num) {
        bits[rand() % 5000000] = rand() % 2;
    }

    difference1 = get_time_in_ms() - current_time;
    current_time = get_time_in_ms();

    for (unsigned int num2 = 0; num2 != 100; ++num2) {
        for (unsigned int num = 0; num != 5000000; ++num) {
            total += bits[num];
        }
    }

    difference2 = get_time_in_ms() - current_time;

    cout << "Bitset:" << endl << "sum total = " << total << ", random access time = " << difference1 << ", sequential access time = " << difference2 << endl << endl;

    delete [] bools;

    cin.get();

    return 0;
}


