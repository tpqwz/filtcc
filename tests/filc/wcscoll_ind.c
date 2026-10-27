#include <wchar.h>

int main(void)
{
    size_t (*fn)(wchar_t *, const wchar_t *, size_t);
    wchar_t dst[4];
    wchar_t src[1];
    src[0] = L'a';
    fn = wcsxfrm;
    (void)fn(dst, src, 4);
    return 0;
}
