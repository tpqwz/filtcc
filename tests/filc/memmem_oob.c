#define _GNU_SOURCE
#include <string.h>

int main(void)
{
    char h[3] = {'a', 'b', 'c'};
    memmem(h, 8, "ab", 2);
    return 0;
}
