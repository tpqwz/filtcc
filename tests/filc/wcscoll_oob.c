#include <wchar.h>

int main(void)
{
    wchar_t src[1];
    src[0] = L'a';
    (void)wcscoll(src, L"b");
    return 0;
}
