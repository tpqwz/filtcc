#include <wchar.h>

int main(void)
{
    char tiny[1];
    mbstate_t st = {0};
    tiny[0] = 0;
    (void)mbrtowc((wchar_t *)tiny, "a", 1, &st);
    return 0;
}
