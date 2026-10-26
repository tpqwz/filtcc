#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Non-variadic wrapper. The pointer caps live in the caller's frame. */
static int wrap_ss(const char *s, const char *fmt, va_list ap)
{
    return vsscanf(s, fmt, ap);
}

static int call_ss(const char *s, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = wrap_ss(s, fmt, ap);
    va_end(ap);
    return r;
}

static int direct_ss(const char *s, ...)
{
    va_list ap;
    int r;
    va_start(ap, s);
    r = vsscanf(s, "%d %3s", ap);
    va_end(ap);
    return r;
}

/* An int vararg consumes a register slot and no pointer capability. */
static int int_then(const char *s, ...)
{
    va_list ap;
    int k, r;
    va_start(ap, s);
    k = va_arg(ap, int);
    r = vsscanf(s, "%d", ap);
    va_end(ap);
    if (k != 3 || r != 1)
        return 1;
    return 0;
}

static int ind_ss(const char *s, const char *fmt, ...)
{
    int (*fn)(const char *, const char *, va_list);
    va_list ap;
    int r;
    fn = vsscanf;
    va_start(ap, fmt);
    r = fn(s, fmt, ap);
    va_end(ap);
    return r;
}

static int wrap_f(FILE *fp, const char *fmt, va_list ap)
{
    return vfscanf(fp, fmt, ap);
}

static int call_f(FILE *fp, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = wrap_f(fp, fmt, ap);
    va_end(ap);
    return r;
}

static int wrap_q(const char *fmt, va_list ap)
{
    return vscanf(fmt, ap);
}

static int call_q(const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = wrap_q(fmt, ap);
    va_end(ap);
    return r;
}

int main(void)
{
    FILE *fp;
    int (*ff)(FILE *, const char *, ...);
    unsigned u;
    int x, n, a, b, c, d, e;
    float f;
    char word[8];
    char path[64];

    fp = tmpfile();
    if (!fp)
        return 1;
    if (fprintf(fp, "42 hello 3.5\n9 8\n11 abc\n22 xyz\n") < 0)
        return 2;
    rewind(fp);
    u = 0;
    word[0] = 0;
    f = 0;
    n = -1;
    if (fscanf(fp, "%u %5s %f %n", &u, word, &f, &n) != 3)
        return 3;
    if (u != 42 || strcmp(word, "hello") != 0 || f != 3.5f || n < 1)
        return 4;

    if (fscanf(fp, "%*d %d", &x) != 1 || x != 8)
        return 6;

    ff = fscanf;
    word[0] = 0;
    if (ff(fp, "%d %3s", &x, word) != 2 || x != 11 || strcmp(word, "abc") != 0)
        return 8;

    word[0] = 0;
    if (call_f(fp, "%d %3s", &x, word) != 2 || x != 22 || strcmp(word, "xyz") != 0)
        return 10;
    fclose(fp);

    x = 0;
    word[0] = 0;
    if (direct_ss("7 hel", &x, word) != 2 || x != 7 || strcmp(word, "hel") != 0)
        return 11;

    u = 0;
    word[0] = 0;
    if (call_ss("15 cat", "%u %3s", &u, word) != 2 || u != 15 || strcmp(word, "cat") != 0)
        return 12;

    /* Fifth pointer is past the integer registers. */
    if (call_ss("1 2 3 4 5", "%d %d %d %d %d", &a, &b, &c, &d, &e) != 5)
        return 13;
    if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5)
        return 14;

    x = 0;
    if (ind_ss("6", "%d", &x) != 1 || x != 6)
        return 15;

    x = 0;
    if (int_then("5", 3, &x) || x != 5)
        return 16;

    snprintf(path, sizeof path, "/tmp/filc-scan-%d", (int)getpid());
    fp = fopen(path, "w");
    if (!fp)
        return 18;
    if (fprintf(fp, "31 dog\n44 fox\n") < 0)
        return 19;
    fclose(fp);
    if (!freopen(path, "r", stdin))
        return 20;
    x = 0;
    word[0] = 0;
    if (scanf("%d %3s", &x, word) != 2 || x != 31 || strcmp(word, "dog") != 0)
        return 21;
    x = 0;
    word[0] = 0;
    if (call_q("%d %3s", &x, word) != 2 || x != 44 || strcmp(word, "fox") != 0)
        return 22;
    remove(path);
    return 0;
}
