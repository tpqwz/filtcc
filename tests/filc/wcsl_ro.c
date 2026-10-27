#include <wchar.h>

int main(void)
{
    (void)wcslcpy((wchar_t *)L"0123456789abcdef", L"ab", 4);
    return 0;
}
