#include <string.h>

int main(void)
{
    char src[1];
    src[0] = 'A';
    (void)rindex(src, 'A');
    return 0;
}
