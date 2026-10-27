#include <string.h>

int main(void)
{
    char dst[2];
    (void)mempcpy(dst, "abcdefgh", 8);
    return 0;
}
