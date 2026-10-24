#include <string.h>
int main(void)
{
    char *(*sf)(const char *, int) = strchr;
    char *c = sf("abc", 'b');
    c[0] = 'Z';
    return 0;
}
