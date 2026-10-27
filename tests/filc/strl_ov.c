#include <string.h>

int main(void)
{
    char dst[4];
    (void)strlcpy(dst, "a", (size_t)-1);
    return 0;
}
