#include <wchar.h>

int main(void)
{
    wchar_t dst[2];
    (void)wcpncpy(dst, L"a", 8);
    return 0;
}
