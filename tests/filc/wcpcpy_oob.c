#include <wchar.h>

int main(void)
{
    wchar_t dst[2];
    (void)wcpcpy(dst, L"abcd");
    return 0;
}
