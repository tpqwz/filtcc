#include <wchar.h>

int main(void)
{
    wchar_t d[8];
    wchar_t s[1];
    s[0] = L'a';
    wmemmove(d, s, 4);
    return 0;
}
