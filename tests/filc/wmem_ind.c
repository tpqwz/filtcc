#include <wchar.h>

int main(void)
{
    wchar_t *(*fn)(wchar_t *, const wchar_t *, size_t) = wmemcpy;
    wchar_t d[1];
    wchar_t s[4];
    s[0] = s[1] = s[2] = s[3] = L'a';
    fn(d, s, 4);
    return 0;
}
