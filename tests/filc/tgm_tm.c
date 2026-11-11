#include <time.h>

int main(void)
{
    char tiny;

    (void)timegm((struct tm *)&tiny);
    return 0;
}
