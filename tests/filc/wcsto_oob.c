#include <wchar.h>

int main(void)
{
    wchar_t b[3];
    b[0] = L'1';
    b[1] = L'2';
    b[2] = L'3';
    return (int)wcstol(b, 0, 10);
}
