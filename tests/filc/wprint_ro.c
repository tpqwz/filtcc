#include <stdio.h>
#include <wchar.h>

int main(void)
{
    swprintf((wchar_t *)L"abcd", 4, L"xy");
    return 0;
}
