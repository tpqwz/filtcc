#include <string.h>

int main(void)
{
    char src[4];
    char dst[4];
    bcopy(src, dst, (size_t)-1);
    return 0;
}
