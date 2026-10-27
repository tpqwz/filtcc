#include <wchar.h>

int main(void)
{
    long (*fn)(const wchar_t *, wchar_t **, int) = wcstol;
    wchar_t b[3];
    b[0] = L'1';
    b[1] = L'2';
    b[2] = L'3';
    return (int)fn(b, 0, 10);
}
