#include <wchar.h>

int main(void)
{
    wchar_t dst[2];
    dst[0] = 0;
    (void)wcslcat(dst, L"a", 8);
    return 0;
}
