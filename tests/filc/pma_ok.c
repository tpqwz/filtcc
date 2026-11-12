#include <stdint.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{
    void *p;
    int n;
    int (*fn)(void **, size_t, size_t);
    p = (void *)1;
    n = posix_memalign(&p, 64, 100);
    if (n != 0 || !p || ((uintptr_t)p & 63))
        return 2;
    memset(p, 0xab, 100);
    free(p);
    n = posix_memalign(&p, 3, 16);
    if (n == 0)
        return 3;
    fn = posix_memalign;
    n = fn(&p, 128, 32);
    if (n != 0 || !p || ((uintptr_t)p & 127))
        return 4;
    memset(p, 0, 32);
    free(p);
    return 0;
}
