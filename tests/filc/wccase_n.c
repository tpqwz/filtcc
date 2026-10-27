#include <wchar.h>

int main(void)
{
    wchar_t src[1];
    src[0] = L'A';
    (void)wcsncasecmp(src, L"a", 2);
    return 0;
}
