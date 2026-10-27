#include <wchar.h>

int main(void)
{
    wchar_t *(*fn)(const wchar_t *, wchar_t);
    wchar_t src[2];
    src[0] = L'a';
    src[1] = L'b';
    fn = wcschr;
    (void)fn(src, L'z');
    return 0;
}
