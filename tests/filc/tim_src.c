#include <time.h>

int main(void)
{
    char tiny;

    (void)time((time_t *)&tiny);
    return 0;
}
