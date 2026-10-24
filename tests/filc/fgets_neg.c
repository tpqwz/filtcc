#include <stdio.h>

int main(void)
{
    char buf[4];
    (void)fgets(buf, -1, stdin);
    return 0;
}
