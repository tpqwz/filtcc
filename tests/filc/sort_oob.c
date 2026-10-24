#include <stdlib.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void)
{
    int a[2] = {1, 2};
    qsort(a, 4, sizeof(int), cmp_int);
    return 0;
}
