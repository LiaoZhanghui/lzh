#include <stdio.h>

template<typename T>
void BubbleSort(T * pArr, int nLen)
{
    for (int i = 0; i < nLen-1; ++i)
    {
        for (int j = 0; j < nLen-i-1; ++j)
        {
            if (pArr[j] > pArr[j+1])
            {
                T temp = pArr[j+1];
                pArr[j+1] = pArr[j];
                pArr[j] = temp;
            }
        }
    }
}

int main()
{
    float arr[10] = {1.1,9.2,6.3,0.4,4.5,3.6,8.7,2.8,5.9,7.01};
    BubbleSort(arr, 10);
    
    for (int i = 0; i < 10; ++i)
    {
        printf("%f\n", arr[i]);
    }
    getchar();
    return 0;
}
