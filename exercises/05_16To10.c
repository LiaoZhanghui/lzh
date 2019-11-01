
#include "stdio.h"


int main()
{
    int input = 0;
    while( scanf("%x", &input) != EOF)
    {
        printf("%d\n", input);
    }

    return 0;
}