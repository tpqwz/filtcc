#include <stdio.h>

int main(void)
{
    char b[3];
    unsigned a, c;
    b[0] = '1';
    b[1] = '.';
    b[2] = '2';
    (void)sscanf(b, "%u.%u", &a, &c);
    return 0;
}
