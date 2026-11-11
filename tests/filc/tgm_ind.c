#include <time.h>

int main(void)
{
    char tiny;
    time_t (*fg)(struct tm *);

    fg = timegm;
    (void)fg((struct tm *)&tiny);
    return 0;
}
