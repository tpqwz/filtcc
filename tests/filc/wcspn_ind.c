#include <wchar.h>

int main(void)
{
    size_t (*fn)(const wchar_t *, const wchar_t *);
    wchar_t src[2];
    src[0] = L'a';
    src[1] = L'b';
    fn = wcsspn;
    (void)fn(src, L"ab");
    return 0;
}
