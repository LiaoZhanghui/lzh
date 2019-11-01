

#include "stdio.h"
#include "stdlib.h"


void Sort(int * arr, int nLen)
{
    int i, j, temp;
    for(i = 0; i < nLen -1; ++i)
    {
        for (j = 0; j < nLen - 1 - i; ++j)
        {
            if (arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    return;
}

int main()
{
    int nLen = 0;
    while (scanf("%d", &nLen)!=EOF)
    {
        int *arr = (int *)malloc(sizeof(int) * nLen);
        for (int i = 0; i < nLen; ++i)
        {
            scanf("%d", arr+i);
        }

        Sort(arr, nLen);
    
        for(int i = 0; i < nLen; ++i)
        {
            if (i != 0 && arr[i] == arr[i-1])
                continue;
            printf("%d\n", arr[i]);
        }

        free(arr);
    }

    return 0;
}



#if 0
int main()
{
    int nLen = 0;
    while (scanf("%d", &nLen)!=EOF)
    {
        int temp = 0;
        int arr[1024] = {0};
        for (int i = 0; i < nLen; ++i)
        {
            scanf("%d", &temp);
            arr[temp] = 1;
        }
        
        for (int i = 0; i < 1024; ++i)
        {
            if (arr[i] == 1)
            {
                printf("%d\n", i);
            }
        }
    }
    
    return 0;
}
#endif
