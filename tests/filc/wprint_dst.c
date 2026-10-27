#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t d[1];
    swprintf(d, 8, L"hi");
    return 0;
}
