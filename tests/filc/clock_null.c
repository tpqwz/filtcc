#include <time.h>
int main(void)
{
    (void)clock_gettime(CLOCK_REALTIME, 0);
    return 0;
}
