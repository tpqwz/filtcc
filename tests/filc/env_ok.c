#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *e;
    char env[] = "FILC_PUTENV=hi";

    if (setenv("FILC_IO_VAR", "ab", 1) != 0)
        return 1;
    e = getenv("FILC_IO_VAR");
    if (!e || e[0] != 'a' || e[1] != 'b' || e[2] != 0)
        return 2;
    e[0] = 'q';
    if (e[0] != 'q')
        return 3;
    e = secure_getenv("FILC_IO_VAR");
    if (!e || e[0] != 'q' || e[1] != 'b')
        return 4;
    if (getenv("FILC_NO_SUCH_VAR_ZZZ") != 0)
        return 5;
    if (putenv(env) != 0)
        return 6;
    e = getenv("FILC_PUTENV");
    if (!e || strcmp(e, "hi") != 0)
        return 7;
    e[0] = 'H';
    /* "FILC_PUTENV=" is 12 bytes; putenv does not copy. */
    if (env[12] != 'H' || strcmp(env, "FILC_PUTENV=Hi") != 0)
        return 8;
    if (unsetenv("FILC_IO_VAR") != 0)
        return 9;
    if (getenv("FILC_IO_VAR") != 0)
        return 10;
    if (unsetenv("FILC_PUTENV") != 0)
        return 11;
    return 0;
}
