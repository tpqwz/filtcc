#include <wchar.h>

static wchar_t *const roend = 0;

int main(void)
{
    return (int)wcstol(L"12", (wchar_t **)&roend, 10);
}
