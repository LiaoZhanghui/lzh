
#include "stdio.h"

int main()
{
    char arr[2][101] = {0};
    gets(arr[1]);
    gets(arr[2]);
    
    printf("%s\n", arr[1]);
    printf("%s\n", arr[2]);
    
    int i = 0;
    while(arr[1][i])
    {
        for(int j = 0; j < 8; ++j)
        {
            printf("%c", arr[1][i+j]);
        }
    }
    
    return 0;
}