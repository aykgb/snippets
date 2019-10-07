/* 
** 使用typedef int arr[num] 时可能会出现的问题及更正 
*/
#include <iostream>

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
}

typedef uint8_t hal_uint8_t;
typedef hal_uint8_t hal_macaddr_t[6];

typedef struct {
	uint8_t mac_bytes[6];
} hal_macaddr_ts;

using namespace std;

int func(hal_macaddr_t var_t)
{
	printf("%d\n", var_t[0]);
	printf("%d\n", var_t[1]);
	printf("%d\n", var_t[7]);
	printf("%d\n", var_t[8]);
	printf("%d\n", var_t[90]);
	
	return sizeof(var_t);
}

int test1_main()
{
	hal_macaddr_t var_t;
	// uint8_t var_t[6];
	printf("size of var_t is : %lld\n\n", sizeof(var_t));
	
	var_t[0] = 99;
	var_t[1] = 100;
	var_t[2] = 101;
	var_t[3] = 102;
	var_t[4] = 103;
	var_t[5] = 104;
	
	
	// cout << var_t[1] << endl;
	int var_i = func(var_t);
	
	printf("\nsize of var_t is : %d\n\n", var_i);
	
	return 0;
}

int func1(hal_macaddr_ts var_ts)
{
	printf("%d\n", var_ts.mac_bytes[0]);
	printf("%d\n", var_ts.mac_bytes[1]);
	printf("%d\n", var_ts.mac_bytes[7]);
	
	return sizeof(var_ts);
}

int test2_main()
{
	hal_macaddr_ts var_ts;
	printf("size of var_ts is : %lld\n\n", sizeof(var_ts));
	var_ts.mac_bytes[0] = 99;
	var_ts.mac_bytes[1] = 100;
	var_ts.mac_bytes[7] = 100;
	
	int var_i = func1(var_ts);
	
	printf("\nsize of var_ts is : %d\n\n", var_i);
	
	return 0;
}

int main()
{
	// test1_main();
	test2_main();
}