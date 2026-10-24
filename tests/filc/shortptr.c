#include <stdlib.h>
int main(void)
{
    int x = 1;
    int **p = malloc(4);
    *p = &x;
    return 0;
}
