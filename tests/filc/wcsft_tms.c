#include <wchar.h>

int main(void)
{
    char tiny;
    wchar_t buf[8];

    (void)wcsftime(buf, 8, L"%Y", (struct tm *)&tiny);
    return 0;
}
