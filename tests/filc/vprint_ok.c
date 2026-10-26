#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int wrap_vs(char *dst, const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vsprintf(dst, fmt, ap);
    va_end(ap);
    return n;
}

static int pass_vs(char *dst, const char *fmt, va_list ap)
{
    return vsprintf(dst, fmt, ap);
}

static int call_pass(char *dst, const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = pass_vs(dst, fmt, ap);
    va_end(ap);
    return n;
}

static int wrap_vsn(char *dst, size_t n, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = vsnprintf(dst, n, fmt, ap);
    va_end(ap);
    return r;
}

static int ind_vs(char *dst, const char *fmt, ...)
{
    int (*fn)(char *, const char *, va_list);
    va_list ap;
    int n;
    fn = vsprintf;
    va_start(ap, fmt);
    n = fn(dst, fmt, ap);
    va_end(ap);
    return n;
}

int main(void)
{
    char buf[64];
    char raw[4] = {'a', 'b', 'c', 'd'};
    char path[64];
    int n = 0;
    int r;
    char c = 0;
    long double ld = 1.5L;
    int (*sp)(char *, const char *, ...);
    int (*snp)(char *, size_t, const char *, ...);
    int (*fp)(FILE *, const char *, ...);
    int (*dp)(int, const char *, ...);
    FILE *out;
    int fd;

    r = wrap_vs(buf, "%d %s", 7, "ok");
    if (r != 4 || strcmp(buf, "7 ok") != 0)
        return 1;
    r = wrap_vs(buf, "%s%s", "ab", "cd");
    if (r != 4 || strcmp(buf, "abcd") != 0)
        return 2;
    r = wrap_vs(buf, "ab%n", &n);
    if (r != 2 || n != 2 || strcmp(buf, "ab") != 0)
        return 3;
    r = wrap_vs(buf, "ab%hhn", &c);
    if (r != 2 || c != 2)
        return 4;
    r = wrap_vs(buf, "%.4s", raw);
    if (r != 4 || memcmp(buf, "abcd", 4) != 0)
        return 5;
    r = wrap_vs(buf, "%.*s", 2, "hello");
    if (r != 2 || strcmp(buf, "he") != 0)
        return 6;
    r = wrap_vs(buf, "%*d%s", 3, 7, "z");
    if (r != 4 || strcmp(buf, "  7z") != 0)
        return 7;
    r = wrap_vs(buf, "%f %s", 1.25, "ok");
    if (r < 3 || strcmp(buf + r - 2, "ok") != 0)
        return 8;
    r = wrap_vs(buf, "%Lf %s", ld, "ok");
    if (r < 3 || strcmp(buf + r - 2, "ok") != 0)
        return 9;
    /* Four ints fill the general registers. %f must not take one,
     * and %Lf must take 16 stack bytes before the string. */
    r = wrap_vs(buf, "%d %d %d %d %f %s", 1, 2, 3, 4, 1.5, "ok");
    if (r < 3 || strcmp(buf + r - 2, "ok") != 0)
        return 26;
    r = wrap_vs(buf, "%d %d %d %d %Lf %s", 1, 2, 3, 4, ld, "ok");
    if (r < 3 || strcmp(buf + r - 2, "ok") != 0)
        return 27;
    r = call_pass(buf, "%s", "hi");
    if (r != 2 || strcmp(buf, "hi") != 0)
        return 10;
    r = wrap_vsn(buf, 4, "hello");
    if (r != 5 || strcmp(buf, "hel") != 0)
        return 11;
    /* An int is not a capability. The last string is a stack argument. */
    r = wrap_vs(buf, "%d %s %s %s %s %s", 1, "a", "b", "c", "d", "e");
    if (r != 11 || strcmp(buf, "1 a b c d e") != 0)
        return 12;
    r = wrap_vs(buf, "%2$s", "no", "yes");
    if (r != 3 || strcmp(buf, "yes") != 0)
        return 13;

    sp = sprintf;
    r = sp(buf, "%d %s", 3, "ok");
    if (r != 4 || strcmp(buf, "3 ok") != 0)
        return 14;
    snp = snprintf;
    r = snp(buf, 8, "%s", "hi");
    if (r != 2 || strcmp(buf, "hi") != 0)
        return 15;
    r = ind_vs(buf, "%s %d", "xy", 9);
    if (r != 4 || strcmp(buf, "xy 9") != 0)
        return 16;
    n = 0;
    r = sp(buf, "ab%n", &n);
    if (r != 2 || n != 2)
        return 17;

    snprintf(path, sizeof path, "/tmp/filc-vp-%d", (int)getpid());
    out = fopen(path, "w+");
    if (!out)
        return 18;
    fp = fprintf;
    r = fp(out, "%s", "hi");
    if (r != 2)
        return 19;
    if (fflush(out) != 0)
        return 20;
    rewind(out);
    if (fread(buf, 1, 8, out) != 2 || memcmp(buf, "hi", 2) != 0)
        return 21;
    fclose(out);
    remove(path);

    fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd < 0)
        return 22;
    dp = dprintf;
    r = dp(fd, "%s", "yz");
    if (r != 2)
        return 23;
    if (lseek(fd, 0, SEEK_SET) != 0)
        return 24;
    if (read(fd, buf, 8) != 2 || memcmp(buf, "yz", 2) != 0)
        return 25;
    close(fd);
    remove(path);
    return 0;
}
