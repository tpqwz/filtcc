#include <stdio.h>

int main(void)
{
    FILE *fp;
    char tiny[4];
    int (*sv)(FILE *, char *, int, size_t);
    fp = tmpfile();
    if (!fp)
        return 1;
    sv = setvbuf;
    sv(fp, tiny, _IOFBF, 64);
    fclose(fp);
    return 0;
}
