#include <stdio.h>

int main(void)
{
    char fmt[2];
    unsigned x;
    fmt[0] = '%';
    fmt[1] = 'u';
    (void)sscanf("1", fmt, &x);
    return 0;
}
