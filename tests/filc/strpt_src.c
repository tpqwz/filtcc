#include <string.h>
#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);

int main(void)
{
    struct tm tm;
    char s[4];

    memset(&tm, 0, sizeof tm);
    s[0] = '2';
    s[1] = '0';
    s[2] = '2';
    s[3] = '0';
    (void)strptime(s, "%Y", &tm);
    return 0;
}
