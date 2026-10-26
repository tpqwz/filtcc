#include <stdio.h>

int main(void)
{
    char tiny;
    (void)sscanf("1", "%u", (unsigned *)&tiny);
    return 0;
}
