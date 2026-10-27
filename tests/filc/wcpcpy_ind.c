#include <wchar.h>

int main(void)
{
    wchar_t *(*fn)(wchar_t *, const wchar_t *);
    wchar_t dst[2];
    fn = wcpcpy;
    (void)fn(dst, L"abcd");
    return 0;
}
