#include <time.h>

int main(void)
{
    char tiny;

    (void)timelocal((struct tm *)&tiny);
    return 0;
}
