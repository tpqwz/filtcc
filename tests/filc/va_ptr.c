#include <stdarg.h>
static int f(int n, ...)
{
    va_list ap;
    int *a, *b, m;
    va_start(ap, n);
    a = va_arg(ap, int *);
    m = va_arg(ap, int);
    b = va_arg(ap, int *);
    va_end(ap);
    if (n != 1 || m != 2)
        return 1;
    if (*a != 11 || *b != 22)
        return 2;
    return 0;
}
int main(void)
{
    int a = 11, b = 22;
    return f(1, &a, 2, &b);
}
