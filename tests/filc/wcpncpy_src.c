#include <wchar.h>

int main(void)
{
    wchar_t dst[8];
    wchar_t src[1];
    src[0] = L'A';
    (void)wcpncpy(dst, src, 4);
    return 0;
}
