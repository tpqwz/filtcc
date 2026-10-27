#include <stdio.h>

int main(void)
{
    FILE *fp;
    int (*sk)(FILE *, long, int);
    long (*tl)(FILE *);
    void (*rw)(FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputc('a', fp) != 'a' || fputc('b', fp) != 'b')
        return 2;
    if (ftell(fp) != 2)
        return 3;
    rewind(fp);
    if (ftell(fp) != 0 || fgetc(fp) != 'a')
        return 4;
    if (fseek(fp, 1, SEEK_SET) != 0 || ftell(fp) != 1 || fgetc(fp) != 'b')
        return 5;
    if (fseek(fp, 0, SEEK_END) != 0 || ftell(fp) != 2)
        return 6;
    if (fseek(fp, -1, SEEK_CUR) != 0 || ftell(fp) != 1)
        return 7;
    if (fseek(fp, 0, SEEK_END) != 0 || fgetc(fp) != EOF || !feof(fp))
        return 8;
    rewind(fp);
    if (feof(fp) || ftell(fp) != 0)
        return 9;
    sk = fseek;
    tl = ftell;
    rw = rewind;
    if (sk(fp, 1, SEEK_SET) != 0 || tl(fp) != 1)
        return 10;
    rw(fp);
    if (tl(fp) != 0 || fgetc(fp) != 'a')
        return 11;
    fclose(fp);
    return 0;
}
