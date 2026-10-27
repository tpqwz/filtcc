#include <stdlib.h>
#include <wchar.h>

int main(void)
{
    char d[2];
    wcstombs(d, L"hello", 8);
    return 0;
}
