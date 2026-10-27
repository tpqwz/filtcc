#include <wchar.h>

int main(void)
{
    wchar_t src[1];
    src[0] = L'A';
    (void)wcscasecmp(L"a", src);
    return 0;
}
