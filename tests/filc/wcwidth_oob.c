#define _XOPEN_SOURCE 700
#include <wchar.h>

int main(void)
{
    wchar_t src[1];
    src[0] = L'a';
    (void)wcswidth(src, 2);
    return 0;
}
