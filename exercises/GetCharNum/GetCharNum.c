

#include "stdio.h"


int GetCharNum(char * str, char ch)
{
    if (!str)
        return 0;
    
    int result = 0;
    while(*str)
    {
        if (*str == ch)
            ++result;
        str++;
    }
    return result;
}

int main()
{
    char str[4096] = {0};
    char ch = 0;

    if (scanf("%[^\n]", str) == EOF)
    {
        printf("input str wrong");
        return 0;
    }
    
    fflush(stdin);
    getchar();
    
    if (scanf("%c", &ch) == EOF)
    {
        printf("input ch wrong");
        return 0;
    }
    fflush(stdin);


    //printf("str is %s, ch is %c\n", str, ch);

    int result = GetCharNum(str, ch);
    printf("%d\n", result);
    return 0;
}