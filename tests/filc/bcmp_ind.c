#include <string.h>

int main(void)
{
    int (*fn)(const void *, const void *, size_t);
    char a[8];
    char b[2];
    fn = bcmp;
    a[0] = 'a';
    b[0] = 'a';
    b[1] = 'b';
    (void)fn(a, b, 4);
    return 0;
}
