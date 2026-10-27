#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    wchar_t fmt[1];
    fmt[0] = L'%';
    swprintf(buf, 8, fmt);
    return 0;
}
