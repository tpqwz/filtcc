#include <stdio.h>

int main(void)
{
    char buf[4];
    (void)fread(buf, (unsigned long)-1, (unsigned long)-1, stdin);
    return 0;
}
