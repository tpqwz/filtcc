#include <stdio.h>

int main(void)
{
    char buf[4];
    (void)fgets(buf, 8, stdin);
    return 0;
}
