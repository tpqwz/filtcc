#include <stdio.h>

int main(void)
{
    char *s = "abcd";
    (void)sscanf("1", "%u", (unsigned *)s);
    return 0;
}
