
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

int main(void)
{
    char str[] = "The quick brown fox";
    
    puts(str);
    
    str_cut(str, 10, 6);    // "The quick [brown ]fox"
    puts(str);              // "The quick fox"
    
    str_cut(str, 3, 6);     // "The [quick ]fox"
    puts(str);              // "The fox"
    
    str_cut(str, 3, -1);    // "The[ fox]"
    puts(str);              // "The"
    
    return 0;
}
