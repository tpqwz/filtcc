#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    wchar_t one[2];
    wchar_t *p;
    FILE *fp;
    wchar_t *(*fn)(wchar_t *, int, FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fwprintf(fp, L"ab\nhello\n") < 0)
        return 2;
    rewind(fp);

    p = fgetws(buf, 8, fp);
    if (p != buf || buf[0] != L'a' || buf[1] != L'b' || buf[2] != L'\n' || buf[3] != 0)
        return 3;

    /* n == 1 stores a NUL and does not consume the next wide character. */
    one[0] = 7;
    one[1] = 7;
    p = fgetws(one, 1, fp);
    if (p != one || one[0] != 0 || one[1] != 7)
        return 4;

    buf[0] = 3;
    if (fgetws(buf, 0, fp) != 0 || buf[0] != 3)
        return 5;

    p = fgetws(buf + 1, 4, fp);
    if (p != buf + 1 || p[0] != L'h' || p[1] != L'e' || p[2] != L'l' || p[3] != 0)
        return 6;

    p = fgetws(buf, 8, fp);
    if (p != buf || buf[0] != L'l' || buf[1] != L'o' || buf[2] != L'\n' || buf[3] != 0)
        return 7;
    if (fgetws(buf, 8, fp) != 0)
        return 8;
    fclose(fp);

    fp = tmpfile();
    if (!fp)
        return 9;
    if (fwprintf(fp, L"Z\n") < 0)
        return 10;
    rewind(fp);
    fn = fgetws;
    p = fn(buf, 8, fp);
    if (p != buf || buf[0] != L'Z' || buf[1] != L'\n' || buf[2] != 0)
        return 11;
    fclose(fp);
    return 0;
}
