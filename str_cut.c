
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

char *replace_char(char *str, char c) {
    for (int i = 0; i <= strlen(str); i++) {
        if (str[i] == c) {
            str[i] = '\0';
        }
    }
    return str;
}

int main(void)
{
    // char str[] = "The quick brown fox";
    
    // puts(str);
    
    // str_cut(str, 10, 6);    // "The quick [brown ]fox"
    // puts(str);              // "The quick fox"
    
    // str_cut(str, 3, 6);     // "The [quick ]fox"
    // puts(str);              // "The fox"
    
    // str_cut(str, 3, -1);    // "The[ fox]"
    // puts(str);              // "The"

    char str2[] = "superMega!";
    replace_char(str2, 'M');
    printf(str2);
    
    return 0;
}
