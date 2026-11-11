#include <time.h>

int main(void)
{
    time_t t = 0;
    struct tm *g = gmtime(&t);
    volatile char c;

    c = ((char *)g)[sizeof(struct tm)];
    (void)c;
    return 0;
}
