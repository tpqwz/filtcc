#include <string.h>

int main(void)
{
    size_t (*fn)(char *, const char *, size_t);
    char dst[4];
    char src[1];
    src[0] = 'a';
    fn = strxfrm;
    (void)fn(dst, src, 4);
    return 0;
}
