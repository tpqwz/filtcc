#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;

    memset(&tm, 0, sizeof tm);
    (void)strftime(0, 8, "%Y", &tm);
    return 0;
}
