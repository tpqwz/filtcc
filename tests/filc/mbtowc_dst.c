#include <stdlib.h>

int main(void)
{
    char tiny[1];
    tiny[0] = 0;
    (void)mbtowc((wchar_t *)tiny, "a", 1);
    return 0;
}
