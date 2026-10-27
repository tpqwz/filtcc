#include <string.h>

int main(void)
{
    char src[1];
    src[0] = 'a';
    (void)strcoll(src, "b");
    return 0;
}
