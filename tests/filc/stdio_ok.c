#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

static int use_vsprintf(char *dst, const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vsprintf(dst, fmt, ap);
    va_end(ap);
    return n;
}

static int use_vsnprintf(char *dst, size_t n, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = vsnprintf(dst, n, fmt, ap);
    va_end(ap);
    return r;
}

static int use_vprintf(const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vprintf(fmt, ap);
    va_end(ap);
    return n;
}

static int use_vfprintf(FILE *fp, const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vfprintf(fp, fmt, ap);
    va_end(ap);
    return n;
}

int main(void)
{
    char buf[16];
    char tiny[4];
    char raw[4] = {'a', 'b', 'c', 'd'};
    int n = 0;
    int r;
    char c = 0;
    char *p;
    int x = 3;
    int **pp;
    int (*sp)(char *, size_t, const char *, ...);

    r = sprintf(buf, "hello");
    if (r != 5 || strcmp(buf, "hello") != 0)
        return 1;
    r = snprintf(tiny, 4, "hello");
    if (r != 5 || memcmp(tiny, "hel", 3) != 0 || tiny[3] != 0)
        return 2;
    r = sprintf(buf, "%s", "hi");
    if (r != 2 || strcmp(buf, "hi") != 0)
        return 3;
    r = sprintf(buf, "ab%n", &n);
    if (r != 2 || n != 2 || strcmp(buf, "ab") != 0)
        return 4;
    r = sprintf(buf, "ab%hhn", &c);
    if (r != 2 || c != 2)
        return 5;
    /* Precision checks those bytes and does not require a NUL. */
    r = sprintf(buf, "%.4s", raw);
    if (r != 4 || memcmp(buf, "abcd", 4) != 0)
        return 6;
    r = sprintf(buf, "%.0s", raw);
    if (r != 0)
        return 7;
    r = sprintf(buf, "%.*s", 2, "hello");
    if (r != 2 || strcmp(buf, "he") != 0)
        return 8;
    r = sprintf(buf, "%*d", 3, 7);
    if (r != 3 || strcmp(buf, "  7") != 0)
        return 9;
    r = sprintf(buf, "%%");
    if (r != 1 || buf[0] != '%')
        return 10;
    /* Positional: the walk stops, and libc still sees every argument. */
    r = sprintf(buf, "%2$s", "no", "yes");
    if (r != 3 || strcmp(buf, "yes") != 0)
        return 11;
    r = snprintf(NULL, 0, "hi");
    if (r != 2)
        return 12;
    r = snprintf(buf, 0, "hi");
    if (r != 2)
        return 13;
    r = use_vsprintf(buf, "ok");
    if (r != 2 || strcmp(buf, "ok") != 0)
        return 14;
    r = use_vsnprintf(tiny, 4, "hello");
    if (r != 5 || strcmp(tiny, "hel") != 0)
        return 15;
    r = use_vsnprintf(NULL, 0, "hi");
    if (r != 2)
        return 16;
    if (printf("%s", "hi\n") < 0)
        return 17;
    if (fprintf(stdout, "%d", 4) != 1)
        return 18;
    if (dprintf(1, "x") != 1)
        return 19;
    if (use_vprintf("%d", 1) != 1)
        return 20;
    if (use_vfprintf(stdout, "%d", 2) != 1)
        return 21;
    if (fflush(NULL) != 0)
        return 22;
    if (fflush(stdout) != 0)
        return 23;
    if (fputc('Z', stdout) == EOF)
        return 24;
    if (putchar('\n') == EOF)
        return 25;
    perror(NULL);
    sp = snprintf;
    r = sp(tiny, 4, "yz");
    if (r != 2 || strcmp(tiny, "yz") != 0)
        return 26;

    p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);
    if (p == MAP_FAILED)
        return 27;
    p[0] = 42;
    if (p[0] != 42)
        return 28;
    pp = (int **)p;
    *pp = &x;
    if (*pp != &x || **pp != 3)
        return 29;
    if (mprotect(p, 4096, PROT_READ) != 0)
        return 30;
    /* Read-only still allows the load, and the stored capability remains. */
    if (*pp != &x || **pp != 3)
        return 31;
    if (munmap(p, 4096) != 0)
        return 32;
    return 0;
}
