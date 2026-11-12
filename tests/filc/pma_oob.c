#include <stdlib.h>
int main(void)
{
    char b[1];
    (void)posix_memalign((void **)b, 16, 16);
    return 0;
}
