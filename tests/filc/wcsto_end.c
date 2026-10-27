#include <wchar.h>

int main(void)
{
    char tiny;
    return (int)wcstol(L"12", (wchar_t **)&tiny, 10);
}
