#include <string.h>

int main(void)
{
    char dst[8];
    char src[1];
    src[0] = 'A';
    (void)strlcpy(dst, src, 2);
    return 0;
}
