#include <string.h>

int main(void)
{
    char src[4];
    src[0] = 'a';
    bcopy(src, 0, 1);
    return 0;
}
