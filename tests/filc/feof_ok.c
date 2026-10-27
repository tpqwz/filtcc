#include <stdio.h>

int main(void)
{
    FILE *fp;
    int (*ef)(FILE *);
    int (*er)(FILE *);
    int (*no)(FILE *);
    void (*cl)(FILE *);
    int fd;

    fp = tmpfile();
    if (!fp)
        return 1;
    if (feof(fp) || ferror(fp))
        return 2;
    if (fputc('x', fp) != 'x')
        return 3;
    rewind(fp);
    if (fgetc(fp) != 'x' || fgetc(fp) != EOF)
        return 4;
    if (!feof(fp) || ferror(fp))
        return 5;
    clearerr(fp);
    if (feof(fp) || ferror(fp))
        return 6;
    fd = fileno(fp);
    if (fd < 0)
        return 7;
    ef = feof;
    er = ferror;
    no = fileno;
    cl = clearerr;
    if (fgetc(fp) != EOF || !ef(fp))
        return 8;
    cl(fp);
    if (ef(fp) || er(fp) || no(fp) != fd)
        return 9;
    fclose(fp);
    return 0;
}
