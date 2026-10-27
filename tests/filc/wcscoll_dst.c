#include <wchar.h>

int main(void)
{
    wchar_t tiny[1];
    tiny[0] = 0;
    (void)wcsxfrm(tiny, L"ab", 2);
    return 0;
}
