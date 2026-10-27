#include <stdio.h>
#include <wchar.h>

int main(void)
{
    fgetws((wchar_t *)L"abcd", 2, stdin);
    return 0;
}
