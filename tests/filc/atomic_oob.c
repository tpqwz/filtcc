#include <stdatomic.h>
#include <stdlib.h>
int main(void)
{
    int x = 1;
    int **p = malloc(4);
    if (!p)
        return 2;
    __atomic_store_n(p, &x, __ATOMIC_SEQ_CST);
    return 0;
}
