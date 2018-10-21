int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

int main(void) {
    char string[] = "string";
    char src[sizeof(string)];
    char substring = str_cut(string, 1, sizeof(string));
    printf(substring);
}


