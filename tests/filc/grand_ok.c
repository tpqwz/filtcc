#include <sys/random.h>
int main(void)
{
    unsigned char buf[8];
    ssize_t n;
    ssize_t (*fn)(void *, size_t, unsigned int);
    n = getrandom(buf, sizeof buf, 0);
    if (n != (ssize_t)sizeof buf)
        return 2;
    fn = getrandom;
    n = fn(buf, 4, 0);
    if (n != 4)
        return 3;
    return 0;
}
