#include <stdlib.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void)
{
    int a[1] = {1};
    qsort(a, (size_t)-1, (size_t)-1, cmp_int);
    return 0;
}
