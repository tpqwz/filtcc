#include <time.h>

int main(void)
{
    time_t t = 0;

    (void)gmtime_r(&t, (struct tm *)"0123456789");
    return 0;
}
