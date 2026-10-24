#include <string.h>
#include <stdlib.h>
int main(void)
{
    char *(*c1)(char *, const char *) = strcpy;
    char *d = malloc(4);
    c1(d, "hello");
    return 0;
}
