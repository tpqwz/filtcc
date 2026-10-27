#include <wchar.h>

int main(void)
{
    char b[8];
    swscanf(L"hi", L"%s", b);
    return 0;
}
