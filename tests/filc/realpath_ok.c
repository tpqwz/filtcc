#include <limits.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{
    char buf[PATH_MAX];
    char *p;
    char *r;
    char *(*fn)(const char *, char *);
    r = realpath("/", buf);
    if (!r || strcmp(r, "/") != 0)
        return 2;
    r = realpath("/no/such/filc_realpath", buf);
    if (r)
        return 3;
    p = realpath("/", 0);
    if (!p || strcmp(p, "/") != 0)
        return 4;
    if (strlen(p) != 1)
        return 5;
    free(p);
    fn = realpath;
    r = fn("/", buf);
    if (!r || strcmp(r, "/") != 0)
        return 6;
    return 0;
}
