#include <wchar.h>

int main(void)
{
    wchar_t a[1], b[1];
    a[0] = L'a';
    b[0] = L'b';
    wmemcmp(a, b, 4);
    return 0;
}
