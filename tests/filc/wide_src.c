#include <stdlib.h>
#include <wchar.h>

int main(void)
{
    char src[2];
    wchar_t d[8];
    src[0] = 'a';
    src[1] = 'b';
    mbstowcs(d, src, 1);
    return 0;
}
