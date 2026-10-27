#include <stdio.h>
#include <wchar.h>

int main(void)
{
    int (*fn)(const wchar_t *, FILE *);
    wchar_t src[2];
    src[0] = L'a';
    src[1] = L'b';
    fn = fputws;
    fn(src, stdout);
    return 0;
}
