#include <stdlib.h>

int main(void)
{
    char b[3];
    b[0] = '1';
    b[1] = '2';
    b[2] = '3';
    return (int)strtol(b, 0, 10);
}
