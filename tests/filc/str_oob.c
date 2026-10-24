#include <string.h>
#include <stdlib.h>
int main(void)
{
    char *d = malloc(4);
    if (!d)
        return 2;
    strcpy(d, "hello");
    return 0;
}
