#include<stdio.h>
#include<string.h>

typedef struct stru_
{
	int a[4];
} myStru;

int main()
{
	myStru stu;
	int a[4][4];
	stu = {{0, 0, 0, 0}};
	int cnt = 0;
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			a[i][j] = cnt++; 
			printf("a[%d][%d] = %d\n", i, j, a[i][j]);
		}
	}
	printf("\n\n\n");
	
	int index = 2;
	memcpy(a[index], stu.a, sizeof(stu.a));
	
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			printf("a[%d][%d] = %d\n", i, j, a[i][j]);
		}
	}
	printf("\n\n\n");

	printf("size of a[2]: %d\n", sizeof(a[2]));
	printf("size of stu.a: %d\n", sizeof(stu.a));
	
}
