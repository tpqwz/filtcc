#include <string.h>

int main(void)
{
    char dst[2];
    dst[0] = 0;
    (void)strlcat(dst, "a", 8);
    return 0;
}
