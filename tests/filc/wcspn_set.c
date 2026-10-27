#include <wchar.h>

int main(void)
{
    wchar_t acc[2];
    acc[0] = L'a';
    acc[1] = L'b';
    (void)wcsspn(L"z", acc);
    return 0;
}
