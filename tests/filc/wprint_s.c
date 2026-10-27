#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[16];
    char b[4] = {'a', 'b', 'c', 'd'};
    swprintf(buf, 16, L"%s", b);
    return 0;
}
