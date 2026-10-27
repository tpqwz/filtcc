#include <stdio.h>

int main(void)
{
    char bad[3] = {'a', 'b', 'c'};
    (void)rename("/tmp/filc-rename-oob", bad);
    return 0;
}
