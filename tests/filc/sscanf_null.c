#include <stdio.h>

int main(void)
{
    (void)sscanf("1", "%u", (unsigned *)0);
    return 0;
}
