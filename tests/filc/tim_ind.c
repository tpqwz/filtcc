#include <time.h>

int main(void)
{
    char tiny;
    time_t (*ft)(time_t *);

    ft = time;
    (void)ft((time_t *)&tiny);
    return 0;
}
