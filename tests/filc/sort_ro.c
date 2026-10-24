#include <stdlib.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void)
{
    const int a[3] = {3, 1, 2};
    qsort((void *)a, 3, sizeof(int), cmp_int);
    return 0;
}
