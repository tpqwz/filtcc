#include <string.h>
#include <time.h>
#include <wchar.h>

int main(void)
{
    struct tm tm;

    memset(&tm, 0, sizeof tm);
    (void)wcsftime(0, 8, L"%Y", &tm);
    return 0;
}
