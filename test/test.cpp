#include <stdio.h>
#include <unistd.h>

int main()
{
    int nTimes = 0;
    while(1)
    {
        printf("nTimes is %d\n", nTimes++);
        sleep(1);
    }
    
    getchar();
    return 0;
}
