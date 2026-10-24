#include <stdlib.h>
int main(void)
{
    void *p = malloc(8);
    if (!p)
        return 2;
    free(p);
    free(p);
    return 0;
}
