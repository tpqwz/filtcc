#include <wchar.h>

int main(void)
{
    wchar_t fmt[2];
    int x;
    fmt[0] = L'%';
    fmt[1] = L'd';
    swscanf(L"1", fmt, &x);
    return 0;
}
