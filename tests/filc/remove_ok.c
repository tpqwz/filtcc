#include <stdio.h>

int main(void)
{
    FILE *fp;
    char src[] = "/tmp/filc-remove-ok-src";
    char dst[] = "/tmp/filc-remove-ok-dst";
    int (*rm)(const char *);
    int (*rn)(const char *, const char *);

    remove(src);
    remove(dst);
    fp = fopen(src, "w");
    if (!fp)
        return 1;
    if (fputc('a', fp) != 'a')
        return 2;
    fclose(fp);
    if (rename(src, dst) != 0)
        return 3;
    if (fopen(src, "r") != 0)
        return 4;
    fp = fopen(dst, "r");
    if (!fp || fgetc(fp) != 'a')
        return 5;
    fclose(fp);
    if (remove(dst) != 0)
        return 6;
    if (fopen(dst, "r") != 0)
        return 7;

    fp = fopen(src, "w");
    if (!fp)
        return 8;
    if (fputc('b', fp) != 'b')
        return 9;
    fclose(fp);
    rm = remove;
    rn = rename;
    if (rn(src, dst) != 0)
        return 10;
    fp = fopen(dst, "r");
    if (!fp || fgetc(fp) != 'b')
        return 11;
    fclose(fp);
    if (rm(dst) != 0)
        return 12;
    if (fopen(dst, "r") != 0)
        return 13;
    return 0;
}
