#include <stdio.h>

int main(void)
{
    char buf[3] = {'a', 'b', 'c'};
    (void)fputs(buf, stdout);
    return 0;
}
