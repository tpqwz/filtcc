#include <wchar.h>

int main(void)
{
    swscanf(L"1", L"%d", (int *)L"abcd");
    return 0;
}
