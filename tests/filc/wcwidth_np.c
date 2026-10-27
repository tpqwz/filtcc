#define _XOPEN_SOURCE 700
#include <wchar.h>

int main(void)
{
    (void)wcswidth(0, 1);
    return 0;
}
