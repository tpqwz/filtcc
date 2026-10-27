#include <stdio.h>

int main(void)
{
    FILE *fp;
    int (*ug)(int, FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputc('b', fp) != 'b')
        return 2;
    rewind(fp);
    if (ungetc('a', fp) != 'a')
        return 3;
    if (fgetc(fp) != 'a')
        return 4;
    if (fgetc(fp) != 'b')
        return 5;
    if (ungetc(EOF, fp) != EOF)
        return 6;
    ug = ungetc;
    if (ug('Z', fp) != 'Z')
        return 7;
    if (fgetc(fp) != 'Z')
        return 8;
    fclose(fp);
    return 0;
}
