#include <inttypes.h>

int main(void)
{
    char b[3];
    char *end;
    b[0] = '1';
    b[1] = '2';
    b[2] = '3';
    (void)strtoimax(b, &end, 10);
    return 0;
}
