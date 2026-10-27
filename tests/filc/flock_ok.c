#include <stdio.h>

int main(void)
{
    FILE *fp;
    void (*lk)(FILE *);
    int (*tr)(FILE *);
    void (*un)(FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputc('a', fp) != 'a')
        return 2;
    flockfile(fp);
    if (ftrylockfile(fp) != 0)
        return 3;
    funlockfile(fp);
    funlockfile(fp);
    rewind(fp);
    if (fgetc(fp) != 'a')
        return 4;
    lk = flockfile;
    tr = ftrylockfile;
    un = funlockfile;
    lk(fp);
    if (tr(fp) != 0)
        return 5;
    un(fp);
    un(fp);
    if (fgetc(fp) != EOF)
        return 6;
    fclose(fp);
    return 0;
}
