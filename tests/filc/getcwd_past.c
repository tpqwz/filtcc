#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char *p = getcwd(NULL, 0);
    size_t n = strlen(p);
    p[n + 1] = 1;
    return 0;
}
