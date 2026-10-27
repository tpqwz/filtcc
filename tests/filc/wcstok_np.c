#include <wchar.h>

int main(void)
{
    wchar_t buf[] = L"a,b";
    (void)wcstok(buf, L",", 0);
    return 0;
}
