
#include<stdlib.h>
#include <stdio.h>
#include<time.h>
#include <unistd.h>

int GenerateRandomNum(int nBegin, int nEnd)
{
    if (nBegin > nEnd)
    {
        printf("input wrong\n");
        return -1;
    }
    else if (nBegin == nEnd)
    {
        return nBegin;
    }

    return (rand() % (nEnd - nBegin + 1) + nBegin);
}

int main()
{
    srand((unsigned int)time(NULL));
    int nRed = 0;
    for (int i = 0; i < 6; ++i)
    {
        nRed = GenerateRandomNum(1,33);
        printf("%02d ", nRed);
    }
    printf("  ");
    int nBlue = GenerateRandomNum(1,16);
    printf("%02d\n", nBlue);
}