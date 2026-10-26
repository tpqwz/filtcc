#include <stdio.h>

int main(void)
{
    char *s = "abcd";
    (void)fscanf(stdin, "%u", (unsigned *)s);
    return 0;
}
