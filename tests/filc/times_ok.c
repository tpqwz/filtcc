#include <sys/times.h>
int main(void)
{
    struct tms t;
    clock_t a, b;
    clock_t (*fn)(struct tms *);
    a = times(&t);
    if (a == (clock_t)-1)
        return 2;
    a = times(0);
    if (a == (clock_t)-1)
        return 3;
    fn = times;
    b = fn(&t);
    if (b == (clock_t)-1 || b < a)
        return 4;
    return 0;
}
