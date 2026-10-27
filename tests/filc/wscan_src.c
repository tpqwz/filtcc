#include <wchar.h>

int main(void)
{
    wchar_t src[2];
    int x;
    src[0] = L'1';
    src[1] = L'2';
    swscanf(src, L"%d", &x);
    return 0;
}
