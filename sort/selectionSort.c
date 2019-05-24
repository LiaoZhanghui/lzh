#include <stdio.h>

template<typename T>
void SelectionSort(T *arr, int len)
{
    for (int i = 0; i < len-1; ++i)
    {
        for (int j = i+1; j < len; ++j)
        {
            if (arr[i] < arr[j])
            {
                T temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

template<typename T>
void SelectionSort2(T *arr, int len)
{
    for (int i = 0; i < len-1; ++i)
    {
		int maxIndex = i;
        for (int j = i+1; j < len; ++j)
        {
            if (arr[maxIndex] < arr[j])
            {
				maxIndex = j;
            }
        }
		if (maxIndex != i)
		{
			T temp = arr[i];
			arr[i] = arr[maxIndex];
			arr[maxIndex] = temp;
		}
    }
}

int main()
{
    int arr[10] = {0,1,9,2,8,3,4,6,5,7};
    SelectionSort2(arr, 10);
    for (int i = 0; i < 10; ++i )
    {
        printf("%d\n", arr[i]);
    }
    getchar();
    return 0;

}
