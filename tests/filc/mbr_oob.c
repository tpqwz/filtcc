#include <wchar.h>

int main(void)
{
    wchar_t wc;
    mbstate_t st = {0};
    char src[1];
    src[0] = 'a';
    (void)mbrtowc(&wc, src, 2, &st);
    return 0;
}
