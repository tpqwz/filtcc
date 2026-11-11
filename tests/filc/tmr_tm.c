#include <time.h>

int main(void)
{
    time_t t = 0;

    (void)localtime_r(&t, 0);
    return 0;
}
