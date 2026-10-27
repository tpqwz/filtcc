#include <wchar.h>

int main(void)
{
    mbstate_t st = {0};
    char src[1];
    src[0] = 'a';
    (void)mbrlen(src, 2, &st);
    return 0;
}
