#include <wchar.h>

int main(void)
{
    wchar_t src[2];
    src[0] = L'a';
    src[1] = L'b';
    (void)wmemchr(src, L'a', 3);
    return 0;
}
