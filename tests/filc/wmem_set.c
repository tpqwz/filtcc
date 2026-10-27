#include <wchar.h>

int main(void)
{
    wchar_t d[1];
    wmemset(d, L'Z', 4);
    return 0;
}
