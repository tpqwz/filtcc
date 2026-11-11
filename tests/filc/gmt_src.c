#include <time.h>

int main(void)
{
    char tiny;

    (void)gmtime((time_t *)&tiny);
    return 0;
}
