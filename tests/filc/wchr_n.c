#include <wchar.h>

int main(void)
{
    wchar_t n[2];
    n[0] = L'a';
    n[1] = L'b';
    (void)wcsstr(L"ab", n);
    return 0;
}
