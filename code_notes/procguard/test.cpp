#include <stdio.h>
#include <unistd.h>

int main()
{
    int nTimes = 0;
    while(nTimes < 20)
    {
        printf("nTimes is %d\n", nTimes++);
        sleep(3);
    }

    int * p = new int;
    *p = 10;
    printf("\nvalue is %d\n", *p);
    delete p;
   // delete p;
    
    getchar();
    return 0;
}
