#include <stdlib.h>
int main(void)
{
    int x = 1;
    realloc(&x, 8);
    return x;
}
