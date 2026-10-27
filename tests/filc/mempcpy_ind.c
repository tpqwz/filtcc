#include <string.h>

int main(void)
{
    void *(*fn)(void *, const void *, size_t);
    char dst[2];
    fn = mempcpy;
    (void)fn(dst, "abcdefgh", 8);
    return 0;
}
