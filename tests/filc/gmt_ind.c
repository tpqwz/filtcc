#include <time.h>

int main(void)
{
    char tiny;
    struct tm *(*fl)(const time_t *);

    fl = localtime;
    (void)fl((time_t *)&tiny);
    return 0;
}
