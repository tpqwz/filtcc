#include <wchar.h>

int main(void)
{
    wchar_t buf[] = L"a,b";
    wchar_t *save = 0;
    (void)wcstok(buf, 0, &save);
    return 0;
}
