#include <stdio.h>
#include <wchar.h>
#include <unistd.h>

int main(void)
{
    char path[64];
    char path2[64];
    FILE *fp;
    wint_t (*fg)(FILE *);
    wint_t (*pg)(wchar_t, FILE *);
    wint_t (*gw)(void);
    wint_t (*pw)(wchar_t);
    wint_t c;

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputwc(L'a', fp) != L'a')
        return 2;
    if (putwc(L'b', fp) != L'b')
        return 3;
    pg = fputwc;
    if (pg(L'c', fp) != L'c')
        return 4;
    rewind(fp);
    if (fgetwc(fp) != L'a')
        return 5;
    if (getwc(fp) != L'b')
        return 6;
    fg = fgetwc;
    if (fg(fp) != L'c')
        return 7;
    if (fgetwc(fp) != WEOF)
        return 8;
    fclose(fp);

    snprintf(path, sizeof path, "/tmp/filc-wg-%d", (int)getpid());
    fp = fopen(path, "w+");
    if (!fp)
        return 9;
    if (fputwc(L'a', fp) != L'a' || fputwc(L'b', fp) != L'b')
        return 10;
    fclose(fp);
    if (!freopen(path, "r", stdin))
        return 11;
    c = getwchar();
    if (c != L'a')
        return 12;
    gw = getwchar;
    if (gw() != L'b' || getwchar() != WEOF)
        return 13;
    fclose(stdin);
    remove(path);

    snprintf(path2, sizeof path2, "/tmp/filc-wp-%d", (int)getpid());
    if (!freopen(path2, "w+", stdout))
        return 14;
    if (putwchar(L'Q') != L'Q')
        return 15;
    pw = putwchar;
    if (pw(L'Z') != L'Z')
        return 16;
    rewind(stdout);
    if (fgetwc(stdout) != L'Q' || getwc(stdout) != L'Z')
        return 17;
    fclose(stdout);
    remove(path2);
    return 0;
}
