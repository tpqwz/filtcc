#include <stdio.h>
#include <wchar.h>

int main(void)
{
    FILE *fp;
    wchar_t buf[2];
    int i;
    wint_t c;

    fp = tmpfile();
    if (!fp)
        return 1;
    fputwc(L'a', fp);
    fputwc(L'b', fp);
    fputwc(L'c', fp);
    fputwc(L'd', fp);
    rewind(fp);
    i = 0;
    while ((c = fgetwc(fp)) != WEOF && i < 4)
        buf[i++] = (wchar_t)c;
    fclose(fp);
    return 0;
}
