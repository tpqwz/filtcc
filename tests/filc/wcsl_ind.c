#include <wchar.h>

int main(void)
{
    size_t (*fn)(wchar_t *, const wchar_t *, size_t);
    wchar_t dst[2];
    fn = wcslcpy;
    (void)fn(dst, L"a", 8);
    return 0;
}
