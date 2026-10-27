#include <wchar.h>

int main(void)
{
    wchar_t buf[] = L"a,b,,c";
    wchar_t only[] = L"###";
    wchar_t lead[] = L",,a";
    wchar_t more[] = L"red-green";
    wchar_t *save = 0;
    wchar_t *sv = 0;
    wchar_t *t;
    wchar_t *(*fn)(wchar_t *, const wchar_t *, wchar_t **);

    t = wcstok(buf, L",", &save);
    if (!t || t[0] != L'a' || t[1] != 0)
        return 1;
    t = wcstok(0, L",", &save);
    if (!t || t[0] != L'b' || t[1] != 0)
        return 2;
    t = wcstok(0, L",", &save);
    if (!t || t[0] != L'c' || t[1] != 0 || save != 0)
        return 3;
    if (wcstok(0, L",", &save) != 0)
        return 4;

    if (wcstok(only, L"#", &sv) != 0)
        return 5;
    t = wcstok(lead, L",", &sv);
    if (!t || t[0] != L'a' || t[1] != 0 || wcstok(0, L",", &sv) != 0)
        return 6;

    fn = wcstok;
    sv = 0;
    t = fn(more, L"-", &sv);
    if (!t || t[0] != L'r' || t[1] != L'e' || t[2] != L'd' || t[3] != 0)
        return 7;
    t = fn(0, L"-", &sv);
    if (!t || t[0] != L'g' || t[-1] != 0 || t[-2] != L'd' || t[5] != 0)
        return 8;
    return 0;
}
