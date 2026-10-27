#include <stdio.h>

int main(void)
{
    FILE *fp;
    fpos_t pos;
    int (*gp)(FILE *, fpos_t *);
    int (*sp)(FILE *, const fpos_t *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputc('a', fp) != 'a' || fputc('b', fp) != 'b')
        return 2;
    rewind(fp);
    if (fgetc(fp) != 'a')
        return 3;
    if (fgetpos(fp, &pos) != 0)
        return 4;
    if (fgetc(fp) != 'b')
        return 5;
    rewind(fp);
    if (fsetpos(fp, &pos) != 0 || fgetc(fp) != 'b')
        return 6;
    gp = fgetpos;
    sp = fsetpos;
    rewind(fp);
    if (gp(fp, &pos) != 0)
        return 7;
    if (fgetc(fp) != 'a')
        return 8;
    if (sp(fp, &pos) != 0 || fgetc(fp) != 'a')
        return 9;
    fclose(fp);
    return 0;
}
