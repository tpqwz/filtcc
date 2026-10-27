#include <stdlib.h>

int main(void)
{
    char src[1];
    src[0] = 'a';
    (void)mblen(src, 2);
    return 0;
}
