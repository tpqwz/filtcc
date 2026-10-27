#include <wchar.h>

int main(void)
{
    char tiny[1];
    tiny[0] = 0;
    (void)mbsinit((mbstate_t *)tiny);
    return 0;
}
