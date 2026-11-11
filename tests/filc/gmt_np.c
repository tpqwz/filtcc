#include <time.h>

int main(void)
{
    /* The failure is the null time_t. */
    (void)gmtime(0);
    return 0;
}
