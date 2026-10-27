#include <string.h>

int main(void)
{
    char dst[2];
    (void)strlcpy(dst, "a", 8);
    return 0;
}
