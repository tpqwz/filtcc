#include <string.h>
int main(void)
{
    char src[4] = {'a', 'b', 'c', 'd'};
    char dst[8];
    strcpy(dst, src);
    return dst[0];
}
