#include <wchar.h>

int main(void)
{
    int (*fn)(const wchar_t *, const wchar_t *);
    wchar_t src[1];
    src[0] = L'A';
    fn = wcscasecmp;
    (void)fn(src, L"a");
    return 0;
}
