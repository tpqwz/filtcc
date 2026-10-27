#include <string.h>

int main(void)
{
    size_t (*fn)(char *, const char *, size_t);
    char dst[2];
    fn = strlcpy;
    (void)fn(dst, "a", 8);
    return 0;
}
