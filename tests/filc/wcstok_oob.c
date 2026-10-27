#include <wchar.h>

int main(void)
{
    wchar_t buf[3];
    wchar_t *save = 0;
    buf[0] = L'a';
    buf[1] = L'b';
    buf[2] = L'c';
    (void)wcstok(buf, L",", &save);
    return 0;
}
