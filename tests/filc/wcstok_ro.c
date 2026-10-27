#include <wchar.h>

int main(void)
{
    wchar_t *save = 0;
    (void)wcstok(L"a,b", L",", &save);
    return 0;
}
