#include <wchar.h>

int main(void)
{
    wchar_t *(*cpy)(wchar_t *, const wchar_t *) = wcscpy;
    wchar_t d[1];
    cpy(d, L"hi");
    return 0;
}
