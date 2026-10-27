#include <string.h>

int main(void)
{
    char src[2];
    char dst[8];
    src[0] = 'a';
    src[1] = 'b';
    bcopy(src, dst, 4);
    return 0;
}
