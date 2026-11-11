#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);

int main(void)
{
    char tiny;

    (void)strptime("2020", "%Y", (struct tm *)&tiny);
    return 0;
}
