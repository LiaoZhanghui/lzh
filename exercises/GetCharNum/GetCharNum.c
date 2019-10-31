

#include "stdio.h"


int GetCharNum(char * str, char ch)
{
    if (!str)
        return 0;
    
    char ch2 = -1;
    
    if (ch >= 'a' && ch <= 'z')
        ch2 = ch + 'A' - 'a';
    else if (ch >= 'A' && ch <= 'Z')
        ch2 = ch + 'a' - 'A';
    
    int result = 0;
    while(*str)
    {
        if (*str == ch || *str == ch2)
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