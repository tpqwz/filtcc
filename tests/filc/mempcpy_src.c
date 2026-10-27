#include <string.h>

int main(void)
{
    char dst[8];
    char src[1];
    src[0] = 'A';
    (void)mempcpy(dst, src, 4);
    return 0;
}
