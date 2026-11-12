#include <sys/select.h>
#include <sys/time.h>
int main(void)
{
    struct timeval tv;
    fd_set set;
    int n;
    int (*fn)(int, fd_set *, fd_set *, fd_set *, struct timeval *);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    n = select(0, 0, 0, 0, &tv);
    if (n != 0)
        return 2;
    FD_ZERO(&set);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    n = select(0, &set, 0, 0, &tv);
    if (n != 0)
        return 3;
    fn = select;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    n = fn(0, 0, 0, 0, &tv);
    if (n != 0)
        return 4;
    return 0;
}
