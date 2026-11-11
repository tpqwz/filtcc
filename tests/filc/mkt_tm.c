#include <time.h>

int main(void)
{
    char tiny;

    (void)mktime((struct tm *)&tiny);
    return 0;
}
