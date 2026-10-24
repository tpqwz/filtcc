#include <stdio.h>

int main(void)
{
    char *s = "hi";
    (void)fgets(s, 2, stdin);
    return 0;
}
