#include <wchar.h>

int main(void)
{
    (void)wcsxfrm((wchar_t *)L"abcd", L"a", 2);
    return 0;
}
