#include <stdio.h>

int main(void)
{
    char buf[4];
    (void)fread(buf, 1, 8, stdin);
    return 0;
}
