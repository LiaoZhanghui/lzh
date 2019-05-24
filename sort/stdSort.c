
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

bool comp(const int& a, const int& b)
{
	return a > b;

	//if (a == 5)
	//	return true;
	//return false;
}

template<typename T>
void stdSort(T *p, int len)
{	
	sort(p, p+len, comp);
}

int main()
{
	int arr[10] = {1,4,7,8,5,2,3,6,9,0};

	for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
	{
		printf("%d ", arr[i]);
	}
	printf("================================\n");

	stdSort(arr, sizeof(arr)/sizeof(arr[0]));	
	for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
	{
		printf("%d\n", arr[i]);
	}
	
	getchar();
	return 0;
}



