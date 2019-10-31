
#include <stdio.h>

int Get1Bit(int num)
{
    int result = 0;
    while(num)
    {
        if (num % 2 == 1)
            result++;
        num /= 2;
    }
    return result;
}

int main()
{
    int test = 0;
    scanf("%d", &test);
    
    int result = Get1Bit(test);
    printf("%d\n", result);
    
    return 0;
}