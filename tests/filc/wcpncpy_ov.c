#include <wchar.h>

int main(void)
{
    wchar_t dst[4];
    (void)wcpncpy(dst, L"a", (size_t)-1);
    return 0;
}
