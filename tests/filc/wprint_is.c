#include <stdio.h>
#include <wchar.h>

int main(void)
{
    int (*sw)(wchar_t *, size_t, const wchar_t *, ...) = swprintf;
    wchar_t buf[16];
    char b[4] = {'a', 'b', 'c', 'd'};
    sw(buf, 16, L"%s", b);
    return 0;
}
