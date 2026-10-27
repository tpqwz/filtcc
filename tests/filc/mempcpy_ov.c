#include <string.h>

int main(void)
{
    char dst[4];
    char src[4];
    (void)mempcpy(dst, src, (size_t)-1);
    return 0;
}
