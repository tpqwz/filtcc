#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);

int main(void)
{
    (void)strptime("2020", "%Y", (struct tm *)"0123456789");
    return 0;
}
