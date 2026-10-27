#include <stdio.h>
#include <wchar.h>

int main(void)
{
    FILE *fp;
    wint_t (*ug)(wint_t, FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputwc(L'b', fp) != L'b')
        return 2;
    rewind(fp);
    if (ungetwc(L'a', fp) != L'a')
        return 3;
    if (fgetwc(fp) != L'a')
        return 4;
    if (fgetwc(fp) != L'b')
        return 5;
    if (ungetwc(WEOF, fp) != WEOF)
        return 6;
    ug = ungetwc;
    if (ug(L'Z', fp) != L'Z')
        return 7;
    if (fgetwc(fp) != L'Z')
        return 8;
    fclose(fp);
    return 0;
}
