#include <string.h>

int main(void)
{
    char tiny[1];
    tiny[0] = 0;
    (void)strxfrm(tiny, "ab", 2);
    return 0;
}
