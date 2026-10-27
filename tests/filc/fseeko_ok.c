#include <stdio.h>

int main(void)
{
    FILE *fp;
    int (*sk)(FILE *, off_t, int);
    off_t (*tl)(FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputc('a', fp) != 'a' || fputc('b', fp) != 'b')
        return 2;
    if (ftello(fp) != 2)
        return 3;
    if (fseeko(fp, 0, SEEK_SET) != 0 || ftello(fp) != 0 || fgetc(fp) != 'a')
        return 4;
    if (fseeko(fp, 1, SEEK_SET) != 0 || ftello(fp) != 1 || fgetc(fp) != 'b')
        return 5;
    if (fseeko(fp, 0, SEEK_END) != 0 || ftello(fp) != 2)
        return 6;
    if (fseeko(fp, -1, SEEK_CUR) != 0 || ftello(fp) != 1)
        return 7;
    if (fseeko(fp, 0, SEEK_END) != 0 || fgetc(fp) != EOF || !feof(fp))
        return 8;
    if (fseeko(fp, 0, SEEK_SET) != 0 || feof(fp) || ftello(fp) != 0)
        return 9;
    sk = fseeko;
    tl = ftello;
    if (sk(fp, 1, SEEK_SET) != 0 || tl(fp) != 1)
        return 10;
    if (sk(fp, 0, SEEK_SET) != 0 || tl(fp) != 0 || fgetc(fp) != 'a')
        return 11;
    fclose(fp);
    return 0;
}
