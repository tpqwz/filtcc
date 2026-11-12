#include <unistd.h>
int main(void)
{
    unsigned char buf[8];
    int n;
    int (*fn)(void *, size_t);
    n = getentropy(buf, sizeof buf);
    if (n != 0)
        return 2;
    fn = getentropy;
    n = fn(buf, 4);
    if (n != 0)
        return 3;
    return 0;
}
