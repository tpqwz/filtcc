#include <inttypes.h>

int main(void)
{
    intmax_t (*fn)(const char *, char **, int);
    char b[3];
    char *end;
    b[0] = '1';
    b[1] = '2';
    b[2] = '3';
    fn = strtoimax;
    (void)fn(b, &end, 10);
    return 0;
}
