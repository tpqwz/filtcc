#include <wchar.h>

int main(void)
{
    wchar_t *(*fn)(wchar_t *, const wchar_t *, wchar_t **);
    wchar_t buf[3];
    wchar_t *save = 0;
    buf[0] = L'a';
    buf[1] = L'b';
    buf[2] = L'c';
    fn = wcstok;
    (void)fn(buf, L",", &save);
    return 0;
}
