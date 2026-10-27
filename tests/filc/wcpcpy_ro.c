#include <wchar.h>

int main(void)
{
    (void)wcpcpy((wchar_t *)L"0123456789abcdef", L"ab");
    return 0;
}
