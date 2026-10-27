#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[2];
    fgetws(buf, 8, stdin);
    return 0;
}
