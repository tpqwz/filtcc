#define _GNU_SOURCE
#include <string.h>

int main(void)
{
    char n[1] = {'x'};
    memmem("hello", 5, n, 4);
    return 0;
}
