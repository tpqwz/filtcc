#include <string.h>
#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);

int main(void)
{
    struct tm tm;

    memset(&tm, 0, sizeof tm);
    (void)strptime(0, "%Y", &tm);
    return 0;
}
