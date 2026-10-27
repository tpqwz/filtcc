#include <string.h>

int main(void)
{
    char src[4];
    src[0] = 'a';
    src[1] = 'b';
    bcopy(src, (char *)"0123456789abcdef", 2);
    return 0;
}
