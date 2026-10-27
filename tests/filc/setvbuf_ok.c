#include <stdio.h>

int main(void)
{
    FILE *fp;
    char buf[64];
    char big[BUFSIZ];
    int (*sv)(FILE *, char *, int, size_t);
    void (*sb)(FILE *, char *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (setvbuf(fp, NULL, _IONBF, 0) != 0)
        return 2;
    if (fputc('a', fp) != 'a')
        return 3;
    rewind(fp);
    if (fgetc(fp) != 'a')
        return 4;
    fclose(fp);

    fp = tmpfile();
    if (!fp)
        return 5;
    if (setvbuf(fp, buf, _IOFBF, sizeof buf) != 0)
        return 6;
    if (fputc('b', fp) != 'b')
        return 7;
    rewind(fp);
    if (fgetc(fp) != 'b')
        return 8;
    fclose(fp);

    fp = tmpfile();
    if (!fp)
        return 9;
    setbuf(fp, NULL);
    if (fputc('c', fp) != 'c')
        return 10;
    rewind(fp);
    if (fgetc(fp) != 'c')
        return 11;
    fclose(fp);

    fp = tmpfile();
    if (!fp)
        return 12;
    setbuf(fp, big);
    if (fputc('d', fp) != 'd')
        return 13;
    rewind(fp);
    if (fgetc(fp) != 'd')
        return 14;
    fclose(fp);

    sv = setvbuf;
    sb = setbuf;
    fp = tmpfile();
    if (!fp)
        return 15;
    if (sv(fp, buf, _IOFBF, sizeof buf) != 0)
        return 16;
    if (fputc('e', fp) != 'e')
        return 17;
    rewind(fp);
    if (fgetc(fp) != 'e')
        return 18;
    fclose(fp);

    fp = tmpfile();
    if (!fp)
        return 19;
    sb(fp, NULL);
    if (fputc('f', fp) != 'f')
        return 20;
    rewind(fp);
    if (fgetc(fp) != 'f')
        return 21;
    fclose(fp);
    return 0;
}
