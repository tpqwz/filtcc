#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t w[2];
    sscanf("ab", "%2ls", w);
    return 0;
}
