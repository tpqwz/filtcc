#include <wchar.h>

int main(void)
{
    char tiny;
    swscanf(L"1", L"%d", (int *)&tiny);
    return 0;
}
