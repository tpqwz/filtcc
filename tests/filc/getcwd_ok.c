#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char buf[4096];
    char big[4096];
    char tiny[1];
    char *p;
    char *q;
    char *(*fn)(char *, size_t);
    size_t n;

    p = getcwd(buf, sizeof buf);
    if (p != buf || !p || p[0] != '/' || !strchr(p, 0))
        return 1;

    /* The return keeps the whole array, not a fresh object at big+8. */
    p = getcwd(big + 8, sizeof big - 8);
    if (p != big + 8 || p[0] != '/')
        return 2;
    p[-1] = 'Z';
    if (big[7] != 'Z')
        return 3;

    q = getcwd(NULL, 0);
    if (!q || q[0] != '/')
        return 4;
    n = strlen(q);
    if (q[n] != 0)
        return 5;
    free(q);

    q = getcwd(NULL, 4096);
    if (!q || q[0] != '/' || strlen(q) >= 4096)
        return 6;
    q[4095] = 0;
    free(q);

    /* In bounds, but smaller than any absolute path. */
    errno = 0;
    if (getcwd(tiny, 1) != NULL || errno != ERANGE)
        return 7;

    fn = getcwd;
    p = fn(buf, sizeof buf);
    if (p != buf || p[0] != '/')
        return 8;
    return 0;
}
