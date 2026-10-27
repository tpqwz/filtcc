#include <stdio.h>

int main(void)
{
    FILE *fp;
    char tiny[8];
    void (*sb)(FILE *, char *);
    fp = tmpfile();
    if (!fp)
        return 1;
    sb = setbuf;
    sb(fp, tiny);
    fclose(fp);
    return 0;
}
