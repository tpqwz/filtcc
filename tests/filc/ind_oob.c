#include <stdlib.h>
int main(void)
{
    void *(*mf)(size_t) = malloc;
    char *p = mf(8);
    p[8] = 1;
    return 0;
}
