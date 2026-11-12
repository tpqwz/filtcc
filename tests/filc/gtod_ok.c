#include <sys/time.h>
int main(void)
{
    struct timeval tv;
    struct timezone tz;
    int n;
    int (*fn)(struct timeval *, struct timezone *);
    n = gettimeofday(&tv, &tz);
    if (n != 0 || tv.tv_sec <= 0)
        return 2;
    n = gettimeofday(&tv, 0);
    if (n != 0 || tv.tv_sec <= 0)
        return 3;
    n = gettimeofday(0, 0);
    if (n != 0)
        return 4;
    fn = gettimeofday;
    n = fn(&tv, 0);
    if (n != 0 || tv.tv_sec <= 0)
        return 5;
    return 0;
}
