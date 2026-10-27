#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[16];
    char tiny[1];
    swprintf(buf, 16, L"%n", tiny);
    return 0;
}
