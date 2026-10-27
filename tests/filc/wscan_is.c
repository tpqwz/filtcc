#include <wchar.h>

int main(void)
{
    int (*fn)(const wchar_t *, const wchar_t *, ...);
    char b[8];
    fn = swscanf;
    fn(L"hi", L"%s", b);
    return 0;
}
