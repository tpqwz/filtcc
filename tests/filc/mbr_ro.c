#include <wchar.h>

int main(void)
{
    wchar_t wc;
    (void)mbrtowc(&wc, "a", 1, (mbstate_t *)"abcdefgh");
    return 0;
}
