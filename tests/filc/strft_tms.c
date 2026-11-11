#include <time.h>

int main(void)
{
    char tiny;
    char buf[8];

    (void)strftime(buf, 8, "%Y", (struct tm *)&tiny);
    return 0;
}
