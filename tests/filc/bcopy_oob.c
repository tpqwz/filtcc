#include <string.h>

int main(void)
{
    char src[8];
    char dst[2];
    memcpy(src, "abcdefgh", 8);
    bcopy(src, dst, 4);
    return 0;
}
