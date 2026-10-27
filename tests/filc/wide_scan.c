#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t w[8];
    sscanf("hello", "%ls", w);
    return 0;
}
