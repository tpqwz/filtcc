#include <stdio.h>

int main(void)
{
    char buf[4] = {'a', 'b', 'c', 'd'};
    (void)fwrite(buf, 1, 8, stdout);
    return 0;
}
