#include <stdio.h>
#include <wchar.h>

int main(void)
{
    FILE *fp;
    wchar_t buf[8];
    wchar_t *p;
    int (*fn)(const wchar_t *, FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fputws(L"ab\n", fp) < 0)
        return 2;
    if (fputws(L"", fp) < 0)
        return 3;
    fn = fputws;
    if (fn(L"Z\n", fp) < 0)
        return 4;
    rewind(fp);
    p = fgetws(buf, 8, fp);
    if (p != buf || buf[0] != L'a' || buf[1] != L'b' || buf[2] != L'\n' || buf[3] != 0)
        return 5;
    p = fgetws(buf, 8, fp);
    if (p != buf || buf[0] != L'Z' || buf[1] != L'\n' || buf[2] != 0)
        return 6;
    fclose(fp);
    return 0;
}
