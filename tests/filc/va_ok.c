#include <stdarg.h>

struct dbl2 {
    double a;
    double b;
};

static int one(int n, ...)
{
    va_list ap;
    int x;
    va_start(ap, n);
    x = va_arg(ap, int);
    va_end(ap);
    return x;
}

static int copied(int n, ...)
{
    va_list ap, aq;
    int x;
    va_start(ap, n);
    va_copy(aq, ap);
    x = va_arg(aq, int);
    va_end(aq);
    va_end(ap);
    return x;
}

/* No named integer reg, so the first slot is the base of the save area. */
static int base_ptr(double d, ...)
{
    va_list ap;
    int *p, n;
    va_start(ap, d);
    p = va_arg(ap, int *);
    n = va_arg(ap, int);
    va_end(ap);
    if (*p != 11 || n != 22)
        return 1;
    return 0;
}

static int stack_vals(int a, int b, int c, int d, int e, int f, ...)
{
    va_list ap;
    int x, y;
    int *p;
    va_start(ap, f);
    x = va_arg(ap, int);
    y = va_arg(ap, int);
    p = va_arg(ap, int *);
    va_end(ap);
    if (x != 70 || y != 80 || *p != 90)
        return 1;
    return 0;
}

static int one_double(int n, ...)
{
    va_list ap;
    double x;
    va_start(ap, n);
    x = va_arg(ap, double);
    va_end(ap);
    if (x != 3.25)
        return 1;
    return 0;
}

/* 16-byte SSE argument: __va_arg memcpy's the two halves together. */
static int sse_pair(int n, ...)
{
    va_list ap;
    struct dbl2 p;
    va_start(ap, n);
    p = va_arg(ap, struct dbl2);
    va_end(ap);
    if (p.a != 1.5 || p.b != 2.5)
        return 1;
    return 0;
}

static int ld_val(int n, ...)
{
    va_list ap;
    long double x;
    va_start(ap, n);
    x = va_arg(ap, long double);
    va_end(ap);
    if ((int)x != 4)
        return 1;
    return 0;
}

int main(void)
{
    int v = 11;
    int s = 90;
    struct dbl2 d;
    d.a = 1.5;
    d.b = 2.5;
    if (one(0, 7) != 7)
        return 1;
    if (copied(0, 7) != 7)
        return 2;
    if (base_ptr(1.0, &v, 22))
        return 3;
    if (stack_vals(1, 2, 3, 4, 5, 6, 70, 80, &s))
        return 4;
    if (one_double(0, 3.25))
        return 5;
    if (sse_pair(0, d))
        return 6;
    if (ld_val(0, 4.0L))
        return 7;
    return 0;
}
