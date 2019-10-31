
#include "stdio.h"
#include "string.h"




int GetLastWorldLen(char * str)
{
    int nLen = strlen(str);
    int i = 0;
    
    for(i = nLen-1; i >= 0; --i)
    {
        if (str[i] == ' ')
        {
            if (i == nLen-1)
            {
                --nLen;
                continue;
            }
            else
            {
                break;
            }
        }
    }
    return nLen - i - 1;
}


int main()
{
    char str[4096] = {0};
    if (scanf("%s", str) == EOF)
    {
        return -1;
    }
    
    int result = GetLastWorldLen(str);
    printf("%d\n", result);
    
    return 0;
}