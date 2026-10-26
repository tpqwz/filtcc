#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *p = getcwd(NULL, 0);
    free(p);
    return p[0];
}
