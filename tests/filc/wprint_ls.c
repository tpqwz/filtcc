#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[16];
    wchar_t w[1];
    w[0] = L'Z';
    swprintf(buf, 16, L"%ls", w);
    return 0;
}
