#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);

int main(void)
{
    (void)strptime("2020", "%Y", 0);
    return 0;
}
