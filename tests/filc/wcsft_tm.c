#include <wchar.h>

int main(void)
{
    wchar_t buf[8];

    (void)wcsftime(buf, 8, L"%Y", 0);
    return 0;
}
