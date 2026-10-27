#include <string.h>

int main(void)
{
    void (*fn)(const void *, void *, size_t);
    char src[8];
    char dst[2];
    fn = bcopy;
    memcpy(src, "abcdefgh", 8);
    fn(src, dst, 4);
    return 0;
}
