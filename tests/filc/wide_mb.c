#include <stdlib.h>
#include <wchar.h>

int main(void)
{
    wchar_t d[1];
    mbstowcs(d, "ab", 4);
    return 0;
}
