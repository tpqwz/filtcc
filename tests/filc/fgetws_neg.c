#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[4];
    fgetws(buf, -1, stdin);
    return 0;
}
