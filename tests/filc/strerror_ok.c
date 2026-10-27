#include <errno.h>
#include <signal.h>
#include <string.h>

static int walk(const char *s)
{
    if (!s)
        return 0;
    while (*s)
        s++;
    return 1;
}

int main(void)
{
    char *(*e)(int);
    char *(*g)(int);
    const char *s, *t;

    s = strerror(0);
    if (!walk(s) || strcmp(s, "Success") != 0)
        return 2;
    t = strerror(ENOENT);
    if (!walk(t) || strcmp(t, "No such file or directory") != 0)
        return 3;
    if (s == t)
        return 4;
    s = strerror(99999);
    if (!walk(s) || strcmp(s, "Unknown error 99999") != 0)
        return 5;
    s = strsignal(0);
    if (!walk(s) || strcmp(s, "Unknown signal 0") != 0)
        return 6;
    s = strsignal(SIGSEGV);
    if (!walk(s) || strcmp(s, "Segmentation fault") != 0)
        return 7;
    e = strerror;
    g = strsignal;
    s = e(ENOENT);
    if (!walk(s) || strcmp(s, "No such file or directory") != 0)
        return 8;
    s = g(SIGSEGV);
    if (!walk(s) || strcmp(s, "Segmentation fault") != 0)
        return 9;
    return 0;
}
