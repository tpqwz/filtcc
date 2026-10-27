#include <stdio.h>

int main(void)
{
    char bad[3] = {'a', 'b', 'c'};
    int (*rn)(const char *, const char *);
    rn = rename;
    (void)rn("/tmp/filc-rename-ind", bad);
    return 0;
}
