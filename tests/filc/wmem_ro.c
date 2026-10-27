#include <wchar.h>

int main(void)
{
    wmemcpy((wchar_t *)L"abcd", L"xy", 1);
    return 0;
}
