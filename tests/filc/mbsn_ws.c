#include <wchar.h>

int main(void)
{
    wchar_t src[2];
    const wchar_t *wp;
    char out[8];
    mbstate_t st = {0};
    src[0] = L'a';
    src[1] = L'b';
    wp = src;
    (void)wcsnrtombs(out, &wp, 3, 8, &st);
    return 0;
}
