#include <stdlib.h>
int main(void)
{
    char *p = malloc(32);
    if (!p)
        return 2;
    realloc(p + 1, 8);
    return 0;
}
