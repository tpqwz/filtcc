#include <time.h>
int main(void)
{
    char b[4];
    (void)clock_gettime(CLOCK_REALTIME, (struct timespec *)b);
    return 0;
}
