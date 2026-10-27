#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t src[2];
    src[0] = L'a';
    src[1] = L'b';
    fputws(src, stdout);
    return 0;
}
