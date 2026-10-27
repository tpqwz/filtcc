#include <string.h>

int main(void)
{
    char src[1];
    src[0] = 'A';
    (void)strchrnul(src, 'Z');
    return 0;
}
