
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
   
    char url[] = "localhost:3000/www.google.com";
    char *hostname;
    hostname = replace_char(url, "/");
    printf(hostname);

    
    return 0;
}


